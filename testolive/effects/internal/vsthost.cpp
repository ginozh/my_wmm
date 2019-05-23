#include "vsthost.h"

#ifndef NOVST

// adapted from http://teragonaudio.com/article/How-to-make-your-own-VST-host.html

#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>

#include "playback/audio.h"
#include "mainwindow.h"
#include "debug.h"

#define BLOCK_SIZE 512
#define CHANNEL_COUNT 2

struct VSTRect {
	int16_t top;
	int16_t left;
	int16_t bottom;
	int16_t right;
};

#define effGetChunk 23
#define effSetChunk 24

// C callbacks
extern "C" {
	// Main host callback
	intptr_t hostCallback(AEffect* effect, int32_t opcode, int32_t index, intptr_t value, void* ptr, float opt) {
		switch(opcode) {
		case audioMasterVersion:
			return 2400;
		case audioMasterIdle:
			effect->dispatcher(effect, effEditIdle, 0, 0, nullptr, 0);
			break;
		case audioMasterGetCurrentProcessLevel:
			return 0;
		// handle other opcodes here... there will be lots of them
		case audioMasterEndEdit: // change made
			break;
		default:
			qInfo() << "Plugin requested unhandled opcode" << opcode;
			break;
		}
		return 0;
	}
}

// Plugin's entry point
typedef AEffect *(*vstPluginFuncPtr)(audioMasterCallback host);
// Plugin's getParameter() method
typedef float (*getParameterFuncPtr)(AEffect *effect, int32_t index);
// Plugin's setParameter() method
typedef void (*setParameterFuncPtr)(AEffect *effect, int32_t index, float value);
// Plugin's processEvents() method
typedef int32_t (*processEventsFuncPtr)(VstEvents *events);
// Plugin's process() method
typedef void (*processFuncPtr)(AEffect *effect, float **inputs, float **outputs, int32_t sampleFrames);

void VSTHost::loadPlugin() {
	QString dll_fn = file_field->get_filename(0, true);

#if defined(__APPLE__)
	bundle = BundleLoad(dll_fn);

	if (bundle == NULL) {
		QMessageBox::critical(nullptr, tr("Error loading VST plugin"), tr("Failed to create VST reference"));
		return;
	}

	vstPluginFuncPtr mainEntryPoint = NULL;
	mainEntryPoint = (vstPluginFuncPtr)CFBundleGetFunctionPointerForName(bundle, CFSTR("VSTPluginMain"));
	// VST plugins previous to the 2.4 SDK used main_macho for the entry point name
	if(mainEntryPoint == NULL) {
		mainEntryPoint = (vstPluginFuncPtr)CFBundleGetFunctionPointerForName(bundle, CFSTR("main_macho"));
	}
	if(mainEntryPoint == NULL) {
		qCritical() << "Couldn't get a pointer to VST plugin's main()";
		BundleClose(bundle);
		return;
	}

	plugin = mainEntryPoint(hostCallback);
	if(plugin == NULL) {
		qCritical() << "Plugin's main() returns null";
		BundleClose(bundle);
		return;
	}
#else
	modulePtr = LibLoad(dll_fn);
	if(modulePtr == nullptr) {
		QString dll_error;

#ifdef _WIN32
		DWORD dll_err = GetLastError();
		dll_error = QString::number(dll_err);
#elif __linux__
		dll_error = dlerror();
#endif
		qCritical() << "Failed to load VST plugin" << dll_fn << "-" << dll_error;

		QString msg_err = tr("Failed to load VST plugin \"%1\": %2").arg(dll_fn, dll_error);

#ifdef _WIN32
		if (dll_err == 193) {
#ifdef _WIN64
			msg_err += "\n\n" + tr("NOTE: You can't load 32-bit VST plugins into a 64-bit build of Olive. Please find a 64-bit version of this plugin or switch to a 32-bit build of Olive.");
#elif _WIN32
			msg_err += "\n\n" + tr("NOTE: You can't load 64-bit VST plugins into a 32-bit build of Olive. Please find a 32-bit version of this plugin or switch to a 64-bit build of Olive.");
#endif
		}
#endif

		QMessageBox::critical(nullptr, tr("Error loading VST plugin"), msg_err);

		return;
	}

	vstPluginFuncPtr mainEntryPoint =
	reinterpret_cast<vstPluginFuncPtr>(LibAddress(modulePtr, "VSTPluginMain"));
	// Instantiate the plugin
	plugin = mainEntryPoint(hostCallback);
#endif
}

void VSTHost::freePlugin() {
	if (plugin != nullptr) {
		stopPlugin();
#if defined(__APPLE__)
		CFBundleUnloadExecutable(bundle);
		CFRelease(bundle);
#else
		LibClose(modulePtr);
#endif
		plugin = nullptr;
	}
}

bool VSTHost::configurePluginCallbacks() {
	// Check plugin's magic number
	// If incorrect, then the file either was not loaded properly, is not a
	// real VST plugin, or is otherwise corrupt.
	if(plugin->magic != kEffectMagic) {
		qCritical() << "Plugin's magic number is bad";
		QMessageBox::critical(mainWindow, tr("VST Error"), tr("Plugin's magic number is invalid"));
		return false;
	}

	// Create dispatcher handle
	dispatcher = reinterpret_cast<dispatcherFuncPtr>(plugin->dispatcher);

	// Set up plugin callback functions
	plugin->getParameter = reinterpret_cast<getParameterFuncPtr>(plugin->getParameter);
	plugin->processReplacing = reinterpret_cast<processFuncPtr>(plugin->processReplacing);
	plugin->setParameter = reinterpret_cast<setParameterFuncPtr>(plugin->setParameter);

	return true;
}

void VSTHost::startPlugin() {
	dispatcher(plugin, effOpen, 0, 0, nullptr, 0.0f);

	// Set some default properties
	dispatcher(plugin, effSetSampleRate, 0, 0, nullptr, current_audio_freq());
	dispatcher(plugin, effSetBlockSize, 0, BLOCK_SIZE, nullptr, 0.0f);

	resumePlugin();
}

void VSTHost::stopPlugin() {
	suspendPlugin();

	dispatcher(plugin, effClose, 0, 0, nullptr, 0);
}

void VSTHost::resumePlugin() {
	dispatcher(plugin, effMainsChanged, 0, 1, nullptr, 0.0f);
}

void VSTHost::suspendPlugin() {
	dispatcher(plugin, effMainsChanged, 0, 0, nullptr, 0.0f);
}

bool VSTHost::canPluginDo(char *canDoString) {
	return (dispatcher(plugin, effCanDo, 0, 0, static_cast<void*>(canDoString), 0.0f) > 0);
}

void VSTHost::processAudio(long numFrames) {
	// Always reset the output array before processing.
	for (int i=0;i<CHANNEL_COUNT;i++) {
		memset(outputs[i], 0, BLOCK_SIZE*sizeof(float));
	}

	plugin->processReplacing(plugin, inputs, outputs, numFrames);
}

VSTHost::VSTHost(Clip* c, const EffectMeta *em) : Effect(c, em) {
	plugin = nullptr;

	inputs = new float* [CHANNEL_COUNT];
	outputs = new float* [CHANNEL_COUNT];
	for(int channel = 0; channel < CHANNEL_COUNT; channel++) {
		inputs[channel] = new float[BLOCK_SIZE];
		outputs[channel] = new float[BLOCK_SIZE];
	}

	file_field = add_row(tr("Plugin"), true, false)->add_field(EFFECT_FIELD_FILE, "filename");
	connect(file_field, SIGNAL(changed()), this, SLOT(change_plugin()));

	EffectRow* interface_row = add_row(tr("Interface"), false, false);
	show_interface_btn = new QPushButton(tr("Show"));
	show_interface_btn->setCheckable(true);
	show_interface_btn->setEnabled(false);
	connect(show_interface_btn, SIGNAL(toggled(bool)), this, SLOT(show_interface(bool)));
	interface_row->add_widget(show_interface_btn);

	dialog = new QDialog(mainWindow);
	dialog->setWindowTitle(tr("VST Plugin"));
	dialog->setAttribute(Qt::WA_NativeWindow, true);
	dialog->setWindowFlags(dialog->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	connect(dialog, SIGNAL(finished(int)), this, SLOT(uncheck_show_button()));
}

VSTHost::~VSTHost() {
	for(int channel = 0; channel < CHANNEL_COUNT; channel++) {
		delete [] inputs[channel];
		delete [] outputs[channel];
	}
	delete [] outputs;
	delete [] inputs;

	freePlugin();
}

void VSTHost::process_audio(double, double, quint8* samples, int nb_bytes, int) {
	if (plugin != nullptr) {
		int interval = BLOCK_SIZE*4;
		for (int i=0;i<nb_bytes;i+=interval) {
			int process_size = qMin(interval, nb_bytes - i);
			int lim = i + process_size;

			// convert to float
			for (int j=i;j<lim;j+=4) {
				qint16 left_sample = qint16(((samples[j+1] & 0xFF) << 8) | (samples[j] & 0xFF));
				qint16 right_sample = qint16(((samples[j+3] & 0xFF) << 8) | (samples[j+2] & 0xFF));

				int index = (j-i)>>2;
				inputs[0][index] = float(left_sample) / float(INT16_MAX);
				inputs[1][index] = float(right_sample) / float(INT16_MAX);
			}

			// send to VST
			processAudio(process_size>>2);

			// convert back to int16
			for (int j=i;j<lim;j+=4) {
				int index = (j-i)>>2;

				qint16 left_sample = qint16(qRound(outputs[0][index] * INT16_MAX));
				qint16 right_sample = qint16(qRound(outputs[1][index] * INT16_MAX));

				samples[j+3] = quint8(right_sample >> 8);
				samples[j+2] = quint8(right_sample);
				samples[j+1] = quint8(left_sample >> 8);
				samples[j] = quint8(left_sample);
			}
		}
	}
}

void VSTHost::custom_load(QXmlStreamReader &stream) {
	if (stream.name() == "plugindata") {
		stream.readNext();
		QByteArray b = QByteArray::fromBase64(stream.text().toUtf8());
		if (plugin != nullptr) {
			dispatcher(plugin, effSetChunk, 0, int32_t(b.size()), static_cast<void*>(b.data()), 0);
		}
	}
}

void VSTHost::save(QXmlStreamWriter &stream) {
	Effect::save(stream);
	if (plugin != nullptr) {
		char* p = nullptr;
		int32_t length = int32_t(dispatcher(plugin, effGetChunk, 0, 0, &p, 0));
		QByteArray b(p, length);
		stream.writeTextElement("plugindata", b.toBase64());
	}
}

void VSTHost::show_interface(bool show) {
	dialog->setVisible(show);
}

void VSTHost::uncheck_show_button() {
	show_interface_btn->setChecked(false);
}

void VSTHost::change_plugin() {
	freePlugin();
	loadPlugin();
	if (plugin != nullptr) {
		if (configurePluginCallbacks()) {
			startPlugin();
#ifdef _WIN32
			dispatcher(plugin, effEditOpen, 0, 0, reinterpret_cast<HWND>(dialog->winId()), 0);
#elif __APPLE__
			dispatcher(plugin, effEditOpen, 0, 0, reinterpret_cast<NSWindow*>(dialog->winId()), 0);
#endif
			VSTRect* eRect = nullptr;
			plugin->dispatcher(plugin, effEditGetRect, 0, 0, &eRect, 0);
			dialog->setFixedWidth(eRect->right);
			dialog->setFixedHeight(eRect->bottom);
		} else {
#ifdef __APPLE__
			CFBundleUnloadExecutable(bundle);
			CFRelease(bundle);
#else
			LibClose(modulePtr);
#endif
			plugin = nullptr;
		}
	}
	show_interface_btn->setEnabled(plugin != nullptr);
}

#endif
