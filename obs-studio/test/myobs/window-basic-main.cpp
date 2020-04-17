/******************************************************************************
    Copyright (C) 2013-2015 by Hugh Bailey <obs.jim@gmail.com>
                               Zachary Lund <admin@computerquip.com>
                               Philippe Groarke <philippe.groarke@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <ctime>
#include <obs.hpp>
#include <QGuiApplication>
#include <QMessageBox>
#include <QShowEvent>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QScreen>
#include <QColorDialog>
#include <QSizePolicy>
#include <QScrollBar>
#include <QTextStream>
#include <QProcess>
#include <util/dstr.h>
#include <util/util.hpp>
#include <util/platform.h>
#include <util/profiler.hpp>
#include <util/dstr.hpp>
#include "moc_predefs.h" // storm
#include "obs-app.hpp"
#include "platform.hpp"
#include "visibility-item-widget.hpp"
#include "item-widget-helpers.hpp"
#include "window-basic-settings.hpp"
#include "window-namedialog.hpp"
#include "window-basic-auto-config.hpp"
#include "window-basic-source-select.hpp"
#include "window-basic-main.hpp"
#include "window-basic-stats.hpp"
#include "window-basic-main-outputs.hpp"
#include "window-log-reply.hpp"
#include "window-projector.hpp"
#include "window-remux.hpp"
#include "qt-wrappers.hpp"
#include "display-helpers.hpp"
#include "volume-control.hpp"
#include "remote-text.hpp"
#include "ui-validation.hpp"
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include "win-update/win-update.hpp"
#include "windows.h"
#endif

#include "ui_OBSBasic.h"
#include "ui_ColorSelect.h"

#include <fstream>
#include <sstream>

#include <QScreen>
#include <QWindow>

#include <json11.hpp>

using namespace json11;
using namespace std;

#ifdef BROWSER_AVAILABLE
#include <browser-panel.hpp>
#endif

#include "ui-config.h"

struct QCef;
struct QCefCookieManager;

QCef *cef = nullptr;
QCefCookieManager *panel_cookies = nullptr;

void DestroyPanelCookieManager();

namespace {

template<typename OBSRef> struct SignalContainer {
	OBSRef ref;
	vector<shared_ptr<OBSSignal>> handlers;
};
}

extern volatile long insideEventLoop;

Q_DECLARE_METATYPE(OBSScene);
Q_DECLARE_METATYPE(OBSSceneItem);
Q_DECLARE_METATYPE(OBSSource);
//Q_DECLARE_METATYPE(obs_order_movement);
Q_DECLARE_METATYPE(SignalContainer<OBSScene>);

QDataStream &operator<<(QDataStream &out, const SignalContainer<OBSScene> &v)
{
	out << v.ref;
	return out;
}

QDataStream &operator>>(QDataStream &in, SignalContainer<OBSScene> &v)
{
	in >> v.ref;
	return in;
}

template<typename T> static T GetOBSRef(QListWidgetItem *item)
{
	return item->data(static_cast<int>(QtDataRole::OBSRef)).value<T>();
}

template<typename T> static void SetOBSRef(QListWidgetItem *item, T &&val)
{
	item->setData(static_cast<int>(QtDataRole::OBSRef),
		      QVariant::fromValue(val));
}

static void AddExtraModulePaths()
{
	char base_module_dir[512];
#if defined(_WIN32) || defined(__APPLE__)
	int ret = GetProgramDataPath(base_module_dir, sizeof(base_module_dir),
				     "obs-studio/plugins/%module%");
#else
	int ret = GetConfigPath(base_module_dir, sizeof(base_module_dir),
				"obs-studio/plugins/%module%");
#endif

	if (ret <= 0)
		return;

	string path = base_module_dir;
#if defined(__APPLE__)
	obs_add_module_path((path + "/bin").c_str(), (path + "/data").c_str());

	BPtr<char> config_bin =
		os_get_config_path_ptr("obs-studio/plugins/%module%/bin");
	BPtr<char> config_data =
		os_get_config_path_ptr("obs-studio/plugins/%module%/data");
	obs_add_module_path(config_bin, config_data);

#elif ARCH_BITS == 64
	obs_add_module_path((path + "/bin/64bit").c_str(),
			    (path + "/data").c_str());
#else
	obs_add_module_path((path + "/bin/32bit").c_str(),
			    (path + "/data").c_str());
#endif
}

extern obs_frontend_callbacks *InitializeAPIInterface(OBSBasic *main);

void assignDockToggle(QDockWidget *dock, QAction *action)
{
	auto handleWindowToggle = [action](bool vis) {
		action->blockSignals(true);
		action->setChecked(vis);
		action->blockSignals(false);
	};
	auto handleMenuToggle = [dock](bool check) {
		dock->blockSignals(true);
		dock->setVisible(check);
		dock->blockSignals(false);
	};

	dock->connect(dock->toggleViewAction(), &QAction::toggled,
		      handleWindowToggle);
	dock->connect(action, &QAction::toggled, handleMenuToggle);
}

extern void RegisterTwitchAuth();
extern void RegisterMixerAuth();
extern void RegisterRestreamAuth();

OBSBasic::OBSBasic(QWidget *parent)
	: OBSMainWindow(parent), ui(new Ui::OBSBasic)
{
	qRegisterMetaTypeStreamOperators<SignalContainer<OBSScene>>(
		"SignalContainer<OBSScene>");
    // previewProgramMode = true; //storm
	setAttribute(Qt::WA_NativeWindow);

#if TWITCH_ENABLED
	RegisterTwitchAuth();
#endif
#if MIXER_ENABLED
	RegisterMixerAuth();
#endif
#if RESTREAM_ENABLED
	RegisterRestreamAuth();
#endif

	setAcceptDrops(true);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
		SLOT(on_customContextMenuRequested(const QPoint &)));

	api = InitializeAPIInterface(this);

	ui->setupUi(this);
	ui->previewDisabledWidget->setVisible(false);

	// startingDockLayout = saveState();
#if 0
	statsDock = new OBSDock();
	statsDock->setObjectName(QStringLiteral("statsDock"));
	statsDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
	statsDock->setWindowTitle(QTStr("Basic.Stats"));
	// addDockWidget(Qt::BottomDockWidgetArea, statsDock);
	statsDock->setVisible(false);
	statsDock->setFloating(true);
	statsDock->resize(700, 200);
#endif
	copyActionsDynamicProperties();

	char styleSheetPath[512];
	int ret = GetProfilePath(styleSheetPath, sizeof(styleSheetPath),
				 "stylesheet.qss");
	if (ret > 0) {
		if (QFile::exists(styleSheetPath)) {
			QString path =
				QString("file:///") + QT_UTF8(styleSheetPath);
			App()->setStyleSheet(path);
		}
	}

	qRegisterMetaType<OBSScene>("OBSScene");
	qRegisterMetaType<OBSSceneItem>("OBSSceneItem");
	qRegisterMetaType<OBSSource>("OBSSource");
	qRegisterMetaType<obs_hotkey_id>("obs_hotkey_id");
	qRegisterMetaType<SavedProjectorInfo *>("SavedProjectorInfo *");

	qRegisterMetaTypeStreamOperators<std::vector<std::shared_ptr<OBSSignal>>>(
		"std::vector<std::shared_ptr<OBSSignal>>");
	qRegisterMetaTypeStreamOperators<OBSScene>("OBSScene");
	qRegisterMetaTypeStreamOperators<OBSSceneItem>("OBSSceneItem");

	ui->scenes->setAttribute(Qt::WA_MacShowFocusRect, false);
	ui->sources->setAttribute(Qt::WA_MacShowFocusRect, false);

	bool sceneGrid = config_get_bool(App()->GlobalConfig(), "BasicWindow",
					 "gridMode");
	ui->scenes->SetGridMode(sceneGrid);

	ui->scenes->setItemDelegate(new SceneRenameDelegate(ui->scenes));

	auto displayResize = [this]() {
		struct obs_video_info ovi;

		if (obs_get_video_info(&ovi))
			ResizePreview(ovi.base_width, ovi.base_height);
	};

	connect(windowHandle(), &QWindow::screenChanged, displayResize);
	connect(ui->preview, &OBSQTDisplay::DisplayResized, displayResize);

	delete shortcutFilter;
	shortcutFilter = CreateShortcutFilter();
	installEventFilter(shortcutFilter);

	stringstream name;
	name << "OBS " << App()->GetVersionString();
	blog(LOG_INFO, "%s", name.str().c_str());
	blog(LOG_INFO, "---------------------------------");

	UpdateTitleBar();

	connect(ui->scenes->itemDelegate(),
		SIGNAL(closeEditor(QWidget *,
				   QAbstractItemDelegate::EndEditHint)),
		this,
		SLOT(SceneNameEdited(QWidget *,
				     QAbstractItemDelegate::EndEditHint)));

#if 0
	cpuUsageInfo = os_cpu_usage_info_start();
	cpuUsageTimer = new QTimer(this);
	connect(cpuUsageTimer.data(), SIGNAL(timeout()), ui->statusbar,
		SLOT(UpdateCPUUsage()));
	cpuUsageTimer->start(3000);
	diskFullTimer = new QTimer(this);
	connect(diskFullTimer, SIGNAL(timeout()), this,
		SLOT(CheckDiskSpaceRemaining()));
#endif
#if 0
	QAction *renameScene = new QAction(ui->scenesDock);
	renameScene->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(renameScene, SIGNAL(triggered()), this, SLOT(EditSceneName()));
	ui->scenesDock->addAction(renameScene);
	QAction *renameSource = new QAction(ui->sourcesDock);
	renameSource->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	connect(renameSource, SIGNAL(triggered()), this,
		SLOT(EditSceneItemName()));
	ui->sourcesDock->addAction(renameSource);
#endif

#ifdef __APPLE__
#if 0
	renameScene->setShortcut({Qt::Key_Return});
	renameSource->setShortcut({Qt::Key_Return});
	ui->actionRemoveSource->setShortcuts({Qt::Key_Backspace});
	ui->actionRemoveScene->setShortcuts({Qt::Key_Backspace});

	ui->action_Settings->setMenuRole(QAction::PreferencesRole);
	ui->actionE_xit->setMenuRole(QAction::QuitRole);
#else
	// renameScene->setShortcut({Qt::Key_F2});
	//renameSource->setShortcut({Qt::Key_F2});
#endif
#endif
	auto addNudge = [this](const QKeySequence &seq, const char *s) {
		QAction *nudge = new QAction(ui->preview);
		nudge->setShortcut(seq);
		nudge->setShortcutContext(Qt::WidgetShortcut);
		ui->preview->addAction(nudge);
		connect(nudge, SIGNAL(triggered()), this, s);
	};

	addNudge(Qt::Key_Up, SLOT(NudgeUp()));
	addNudge(Qt::Key_Down, SLOT(NudgeDown()));
	addNudge(Qt::Key_Left, SLOT(NudgeLeft()));
	addNudge(Qt::Key_Right, SLOT(NudgeRight()));

	// assignDockToggle(ui->scenesDock, ui->toggleScenes);
	//assignDockToggle(ui->sourcesDock, ui->toggleSources);
	//assignDockToggle(ui->mixerDock, ui->toggleMixer);
	//assignDockToggle(ui->transitionsDock, ui->toggleTransitions);
	//assignDockToggle(ui->controlsDock, ui->toggleControls);
#if 0
	assignDockToggle(statsDock, ui->toggleStats);

	//hide all docking panes
	ui->toggleScenes->setChecked(false);
	ui->toggleSources->setChecked(false);
	ui->toggleMixer->setChecked(false);
	//ui->toggleTransitions->setChecked(false);
	ui->toggleControls->setChecked(false);
	ui->toggleStats->setChecked(false);
#endif
	QPoint curPos;

	//restore parent window geometry
	const char *geometry = config_get_string(App()->GlobalConfig(),
						 "BasicWindow", "geometry");
	if (geometry != NULL) {
		QByteArray byteArray =
			QByteArray::fromBase64(QByteArray(geometry));
		restoreGeometry(byteArray);

		QRect windowGeometry = normalGeometry();
		if (!WindowPositionValid(windowGeometry)) {
			QRect rect = App()->desktop()->geometry();
			setGeometry(QStyle::alignedRect(Qt::LeftToRight,
							Qt::AlignCenter, size(),
							rect));
		}

		curPos = pos();
	} else {
		QRect desktopRect =
			QGuiApplication::primaryScreen()->geometry();
		QSize adjSize = desktopRect.size() / 2 - size() / 2;
		curPos = QPoint(adjSize.width(), adjSize.height());
	}
#if 0
	QPoint curSize(width(), height());
	QPoint statsDockSize(statsDock->width(), statsDock->height());
	QPoint statsDockPos = curSize / 2 - statsDockSize / 2;
	QPoint newPos = curPos + statsDockPos;
	statsDock->move(newPos);
#endif
	// ui->previewLabel->setProperty("themeID", "previewProgramLabels");

	bool labels = config_get_bool(GetGlobalConfig(), "BasicWindow",
				      "StudioModeLabels");
#if 0
	if (!previewProgramMode)
		ui->previewLabel->setHidden(true);
	else
		ui->previewLabel->setHidden(!labels);
#endif
	ui->previewDisabledWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->previewDisabledWidget,
		SIGNAL(customContextMenuRequested(const QPoint &)), this,
		SLOT(PreviewDisabledMenu(const QPoint &)));
	connect(ui->enablePreviewButton, SIGNAL(clicked()), this,
		SLOT(TogglePreview()));

	connect(ui->scenes->model(),
		SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)),
		this,
		SLOT(ScenesReordered(const QModelIndex &, int, int,
				     const QModelIndex &, int)));
}

static void SaveAudioDevice(const char *name, int channel, obs_data_t *parent,
			    vector<OBSSource> &audioSources)
{
	obs_source_t *source = obs_get_output_source(channel);
	if (!source)
		return;

	audioSources.push_back(source);

	obs_data_t *data = obs_save_source(source);

	obs_data_set_obj(parent, name, data);

	obs_data_release(data);
	obs_source_release(source);
}

static obs_data_t *GenerateSaveData(obs_data_array_t *sceneOrder,
				    obs_data_array_t *quickTransitionData,
				    int transitionDuration,
				    obs_data_array_t *transitions,
				    OBSScene &scene, OBSSource &curProgramScene,
				    obs_data_array_t *savedProjectorList)
{
	obs_data_t *saveData = obs_data_create();

	vector<OBSSource> audioSources;
	audioSources.reserve(6);

	SaveAudioDevice(DESKTOP_AUDIO_1, 1, saveData, audioSources);
	SaveAudioDevice(DESKTOP_AUDIO_2, 2, saveData, audioSources);
	SaveAudioDevice(AUX_AUDIO_1, 3, saveData, audioSources);
	SaveAudioDevice(AUX_AUDIO_2, 4, saveData, audioSources);
	SaveAudioDevice(AUX_AUDIO_3, 5, saveData, audioSources);
	SaveAudioDevice(AUX_AUDIO_4, 6, saveData, audioSources);

	/* -------------------------------- */
	/* save non-group sources           */

	auto FilterAudioSources = [&](obs_source_t *source) {
		if (obs_source_is_group(source))
			return false;

		return find(begin(audioSources), end(audioSources), source) ==
		       end(audioSources);
	};
	using FilterAudioSources_t = decltype(FilterAudioSources);

	obs_data_array_t *sourcesArray = obs_save_sources_filtered(
		[](void *data, obs_source_t *source) {
			return (*static_cast<FilterAudioSources_t *>(data))(
				source);
		},
		static_cast<void *>(&FilterAudioSources));

	/* -------------------------------- */
	/* save group sources separately    */

	/* saving separately ensures they won't be loaded in older versions */
	obs_data_array_t *groupsArray = obs_save_sources_filtered(
		[](void *, obs_source_t *source) {
			return obs_source_is_group(source);
		},
		nullptr);

	/* -------------------------------- */

	obs_source_t *transition = obs_get_output_source(0);
	obs_source_t *currentScene = obs_scene_get_source(scene);
	const char *sceneName = obs_source_get_name(currentScene);
	const char *programName = obs_source_get_name(curProgramScene);

	const char *sceneCollection = config_get_string(
		App()->GlobalConfig(), "Basic", "SceneCollection");

	obs_data_set_string(saveData, "current_scene", sceneName);
	obs_data_set_string(saveData, "current_program_scene", programName);
	obs_data_set_array(saveData, "scene_order", sceneOrder);
	obs_data_set_string(saveData, "name", sceneCollection);
	obs_data_set_array(saveData, "sources", sourcesArray);
	obs_data_set_array(saveData, "groups", groupsArray);
	obs_data_set_array(saveData, "quick_transitions", quickTransitionData);
	obs_data_set_array(saveData, "transitions", transitions);
	obs_data_set_array(saveData, "saved_projectors", savedProjectorList);
	obs_data_array_release(sourcesArray);
	obs_data_array_release(groupsArray);

	obs_data_set_string(saveData, "current_transition",
			    obs_source_get_name(transition));
	obs_data_set_int(saveData, "transition_duration", transitionDuration);
	obs_source_release(transition);

	return saveData;
}

void OBSBasic::copyActionsDynamicProperties()
{
#if 0
	// Themes need the QAction dynamic properties
	for (QAction *x : ui->scenesToolbar->actions()) {
		QWidget *temp = ui->scenesToolbar->widgetForAction(x);

		for (QByteArray &y : x->dynamicPropertyNames()) {
			temp->setProperty(y, x->property(y));
		}
	}

	for (QAction *x : ui->sourcesToolbar->actions()) {
		QWidget *temp = ui->sourcesToolbar->widgetForAction(x);

		for (QByteArray &y : x->dynamicPropertyNames()) {
			temp->setProperty(y, x->property(y));
		}
	}
#endif
}

void OBSBasic::UpdateVolumeControlsDecayRate()
{
	double meterDecayRate =
		config_get_double(basicConfig, "Audio", "MeterDecayRate");

	for (size_t i = 0; i < volumes.size(); i++) {
		volumes[i]->SetMeterDecayRate(meterDecayRate);
	}
}

void OBSBasic::UpdateVolumeControlsPeakMeterType()
{
	uint32_t peakMeterTypeIdx =
		config_get_uint(basicConfig, "Audio", "PeakMeterType");

	enum obs_peak_meter_type peakMeterType;
	switch (peakMeterTypeIdx) {
	case 0:
		peakMeterType = SAMPLE_PEAK_METER;
		break;
	case 1:
		peakMeterType = TRUE_PEAK_METER;
		break;
	default:
		peakMeterType = SAMPLE_PEAK_METER;
		break;
	}

	for (size_t i = 0; i < volumes.size(); i++) {
		volumes[i]->setPeakMeterType(peakMeterType);
	}
}

void OBSBasic::ClearVolumeControls()
{
	for (VolControl *vol : volumes)
		delete vol;

	volumes.clear();
}

obs_data_array_t *OBSBasic::SaveSceneListOrder()
{
	obs_data_array_t *sceneOrder = obs_data_array_create();

	for (int i = 0; i < ui->scenes->count(); i++) {
		obs_data_t *data = obs_data_create();
		obs_data_set_string(data, "name",
				    QT_TO_UTF8(ui->scenes->item(i)->text()));
		obs_data_array_push_back(sceneOrder, data);
		obs_data_release(data);
	}

	return sceneOrder;
}

obs_data_array_t *OBSBasic::SaveProjectors()
{
	obs_data_array_t *savedProjectors = obs_data_array_create();

	auto saveProjector = [savedProjectors](OBSProjector *projector) {
		if (!projector)
			return;

		obs_data_t *data = obs_data_create();
		ProjectorType type = projector->GetProjectorType();
		switch (type) {
		case ProjectorType::Scene:
		case ProjectorType::Source: {
			obs_source_t *source = projector->GetSource();
			const char *name = obs_source_get_name(source);
			obs_data_set_string(data, "name", name);
			break;
		}
		default:
			break;
		}
		obs_data_set_int(data, "monitor", projector->GetMonitor());
		obs_data_set_int(data, "type", static_cast<int>(type));
		obs_data_set_string(
			data, "geometry",
			projector->saveGeometry().toBase64().constData());
		obs_data_array_push_back(savedProjectors, data);
		obs_data_release(data);
	};

	for (size_t i = 0; i < projectors.size(); i++)
		saveProjector(static_cast<OBSProjector *>(projectors[i]));

	return savedProjectors;
}

void OBSBasic::Save(const char *file)
{
	OBSScene scene = GetCurrentScene();
	OBSSource curProgramScene = OBSGetStrongRef(programScene);
	if (!curProgramScene)
		curProgramScene = obs_scene_get_source(scene);

	obs_data_array_t *sceneOrder = SaveSceneListOrder();
	obs_data_array_t *transitions = SaveTransitions();
	obs_data_array_t *quickTrData = SaveQuickTransitions();
	obs_data_array_t *savedProjectorList = SaveProjectors();
	obs_data_t *saveData = GenerateSaveData(
		sceneOrder, quickTrData, iTransitionDuration/*ui->transitionDuration->value()*/,
		transitions, scene, curProgramScene, savedProjectorList);

	obs_data_set_bool(saveData, "preview_locked", ui->preview->Locked());
	obs_data_set_bool(saveData, "scaling_enabled",
			  ui->preview->IsFixedScaling());
	obs_data_set_int(saveData, "scaling_level",
			 ui->preview->GetScalingLevel());
	obs_data_set_double(saveData, "scaling_off_x",
			    ui->preview->GetScrollX());
	obs_data_set_double(saveData, "scaling_off_y",
			    ui->preview->GetScrollY());

	if (api) {
		obs_data_t *moduleObj = obs_data_create();
		api->on_save(moduleObj);
		obs_data_set_obj(saveData, "modules", moduleObj);
		obs_data_release(moduleObj);
	}

	if (!obs_data_save_json_safe(saveData, file, "tmp", "bak"))
		blog(LOG_ERROR, "Could not save scene data to %s", file);

	obs_data_release(saveData);
	obs_data_array_release(sceneOrder);
	obs_data_array_release(quickTrData);
	obs_data_array_release(transitions);
	obs_data_array_release(savedProjectorList);
}

void OBSBasic::DeferSaveBegin()
{
	os_atomic_inc_long(&disableSaving);
}

void OBSBasic::DeferSaveEnd()
{
	long result = os_atomic_dec_long(&disableSaving);
	if (result == 0) {
		SaveProject();
	}
}

static void LoadAudioDevice(const char *name, int channel, obs_data_t *parent)
{
	obs_data_t *data = obs_data_get_obj(parent, name);
	if (!data)
		return;

	obs_source_t *source = obs_load_source(data);
	if (source) {
		obs_set_output_source(channel, source);
		obs_source_release(source);
	}

	obs_data_release(data);
}

static inline bool HasAudioDevices(const char *source_id)
{
	const char *output_id = source_id;
	obs_properties_t *props = obs_get_source_properties(output_id);
	size_t count = 0;

	if (!props)
		return false;

	obs_property_t *devices = obs_properties_get(props, "device_id");
	if (devices)
		count = obs_property_list_item_count(devices);

	obs_properties_destroy(props);

	return count != 0;
}

void OBSBasic::CreateFirstRunSources()
{
	bool hasDesktopAudio = HasAudioDevices(App()->OutputAudioSource());
	bool hasInputAudio = HasAudioDevices(App()->InputAudioSource());

	if (hasDesktopAudio)
		ResetAudioDevice(App()->OutputAudioSource(), "default",
				 Str("Basic.DesktopDevice1"), 1);
	if (hasInputAudio)
		ResetAudioDevice(App()->InputAudioSource(), "default",
				 Str("Basic.AuxDevice1"), 3);
}

void OBSBasic::CreateDefaultScene(bool firstStart)
{
	disableSaving++;

	ClearSceneData();
	InitDefaultTransitions();
	CreateDefaultQuickTransitions();
	// ui->transitionDuration->setValue(300);
    iTransitionDuration = 300;
	SetTransition(fadeTransition);

	obs_scene_t *scene = obs_scene_create(Str("Basic.Scene"));

	if (firstStart)
		CreateFirstRunSources();

	SetCurrentScene(scene, true);
	obs_scene_release(scene);

	disableSaving--;
}

static void ReorderItemByName(QListWidget *lw, const char *name, int newIndex)
{
	for (int i = 0; i < lw->count(); i++) {
		QListWidgetItem *item = lw->item(i);

		if (strcmp(name, QT_TO_UTF8(item->text())) == 0) {
			if (newIndex != i) {
				item = lw->takeItem(i);
				lw->insertItem(newIndex, item);
			}
			break;
		}
	}
}

void OBSBasic::LoadSceneListOrder(obs_data_array_t *array)
{
	size_t num = obs_data_array_count(array);

	for (size_t i = 0; i < num; i++) {
		obs_data_t *data = obs_data_array_item(array, i);
		const char *name = obs_data_get_string(data, "name");

		ReorderItemByName(ui->scenes, name, (int)i);

		obs_data_release(data);
	}
}

void OBSBasic::LoadSavedProjectors(obs_data_array_t *array)
{
	for (SavedProjectorInfo *info : savedProjectorsArray) {
		delete info;
	}
	savedProjectorsArray.clear();

	size_t num = obs_data_array_count(array);

	for (size_t i = 0; i < num; i++) {
		obs_data_t *data = obs_data_array_item(array, i);

		SavedProjectorInfo *info = new SavedProjectorInfo();
		info->monitor = obs_data_get_int(data, "monitor");
		info->type = static_cast<ProjectorType>(
			obs_data_get_int(data, "type"));
		info->geometry =
			std::string(obs_data_get_string(data, "geometry"));
		info->name = std::string(obs_data_get_string(data, "name"));
		savedProjectorsArray.emplace_back(info);

		obs_data_release(data);
	}
}

static void LogFilter(obs_source_t *, obs_source_t *filter, void *v_val)
{
	const char *name = obs_source_get_name(filter);
	const char *id = obs_source_get_id(filter);
	int val = (int)(intptr_t)v_val;
	string indent;

	for (int i = 0; i < val; i++)
		indent += "    ";

	blog(LOG_INFO, "%s- filter: '%s' (%s)", indent.c_str(), name, id);
}

static bool LogSceneItem(obs_scene_t *, obs_sceneitem_t *item, void *v_val)
{
	obs_source_t *source = obs_sceneitem_get_source(item);
	const char *name = obs_source_get_name(source);
	const char *id = obs_source_get_id(source);
	int indent_count = (int)(intptr_t)v_val;
	string indent;

	for (int i = 0; i < indent_count; i++)
		indent += "    ";

	blog(LOG_INFO, "%s- source: '%s' (%s)", indent.c_str(), name, id);

	obs_monitoring_type monitoring_type =
		obs_source_get_monitoring_type(source);

	if (monitoring_type != OBS_MONITORING_TYPE_NONE) {
		const char *type =
			(monitoring_type == OBS_MONITORING_TYPE_MONITOR_ONLY)
				? "monitor only"
				: "monitor and output";

		blog(LOG_INFO, "    %s- monitoring: %s", indent.c_str(), type);
	}
	int child_indent = 1 + indent_count;
	obs_source_enum_filters(source, LogFilter,
				(void *)(intptr_t)child_indent);
	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, LogSceneItem,
					       (void *)(intptr_t)child_indent);
	return true;
}

void OBSBasic::LogScenes()
{
	blog(LOG_INFO, "------------------------------------------------");
	blog(LOG_INFO, "Loaded scenes:");

	for (int i = 0; i < ui->scenes->count(); i++) {
		QListWidgetItem *item = ui->scenes->item(i);
		OBSScene scene = GetOBSRef<OBSScene>(item);

		obs_source_t *source = obs_scene_get_source(scene);
		const char *name = obs_source_get_name(source);

		blog(LOG_INFO, "- scene '%s':", name);
		obs_scene_enum_items(scene, LogSceneItem, (void *)(intptr_t)1);
		obs_source_enum_filters(source, LogFilter, (void *)(intptr_t)1);
	}

	blog(LOG_INFO, "------------------------------------------------");
}

void OBSBasic::Load(const char *file)
{
	disableSaving++;

	obs_data_t *data = obs_data_create_from_json_file_safe(file, "bak");
	if (!data) {
		disableSaving--;
		blog(LOG_INFO, "No scene file found, creating default scene");
		CreateDefaultScene(true);
		SaveProject();
		return;
	}

	ClearSceneData();
	InitDefaultTransitions();

	obs_data_t *modulesObj = obs_data_get_obj(data, "modules");
	if (api)
		api->on_preload(modulesObj);

	obs_data_array_t *sceneOrder = obs_data_get_array(data, "scene_order");
	obs_data_array_t *sources = obs_data_get_array(data, "sources");
	obs_data_array_t *groups = obs_data_get_array(data, "groups");
	obs_data_array_t *transitions = obs_data_get_array(data, "transitions");
	const char *sceneName = obs_data_get_string(data, "current_scene");
	const char *programSceneName =
		obs_data_get_string(data, "current_program_scene");
	const char *transitionName =
		obs_data_get_string(data, "current_transition");

	if (!opt_starting_scene.empty()) {
		programSceneName = opt_starting_scene.c_str();
		if (!IsPreviewProgramMode())
			sceneName = opt_starting_scene.c_str();
	}

	int newDuration = obs_data_get_int(data, "transition_duration");
	if (!newDuration)
		newDuration = 300;

	if (!transitionName)
		transitionName = obs_source_get_name(fadeTransition);

	const char *curSceneCollection = config_get_string(
		App()->GlobalConfig(), "Basic", "SceneCollection");

	obs_data_set_default_string(data, "name", curSceneCollection);

	const char *name = obs_data_get_string(data, "name");
	obs_source_t *curScene;
	obs_source_t *curProgramScene;
	obs_source_t *curTransition;

	if (!name || !*name)
		name = curSceneCollection;

	LoadAudioDevice(DESKTOP_AUDIO_1, 1, data);
	LoadAudioDevice(DESKTOP_AUDIO_2, 2, data);
	LoadAudioDevice(AUX_AUDIO_1, 3, data);
	LoadAudioDevice(AUX_AUDIO_2, 4, data);
	LoadAudioDevice(AUX_AUDIO_3, 5, data);
	LoadAudioDevice(AUX_AUDIO_4, 6, data);

	if (!sources) {
		sources = groups;
		groups = nullptr;
	} else {
		obs_data_array_push_back_array(sources, groups);
	}

	obs_load_sources(sources, nullptr, nullptr);

	if (transitions)
		LoadTransitions(transitions);
	if (sceneOrder)
		LoadSceneListOrder(sceneOrder);

	obs_data_array_release(transitions);

	curTransition = FindTransition(transitionName);
	if (!curTransition)
		curTransition = fadeTransition;

	// ui->transitionDuration->setValue(newDuration);
    iTransitionDuration=newDuration;
	SetTransition(curTransition);

retryScene:
	curScene = obs_get_source_by_name(sceneName);
	curProgramScene = obs_get_source_by_name(programSceneName);

	/* if the starting scene command line parameter is bad at all,
	 * fall back to original settings */
	if (!opt_starting_scene.empty() && (!curScene || !curProgramScene)) {
		sceneName = obs_data_get_string(data, "current_scene");
		programSceneName =
			obs_data_get_string(data, "current_program_scene");
		obs_source_release(curScene);
		obs_source_release(curProgramScene);
		opt_starting_scene.clear();
		goto retryScene;
	}

	if (!curProgramScene) {
		curProgramScene = curScene;
		obs_source_addref(curScene);
	}

	SetCurrentScene(curScene, true);
	if (IsPreviewProgramMode())
		TransitionToScene(curProgramScene, true);
	obs_source_release(curScene);
	obs_source_release(curProgramScene);

	obs_data_array_release(sources);
	obs_data_array_release(groups);
	obs_data_array_release(sceneOrder);

	/* ------------------- */

	bool projectorSave = config_get_bool(GetGlobalConfig(), "BasicWindow",
					     "SaveProjectors");

	if (projectorSave) {
		obs_data_array_t *savedProjectors =
			obs_data_get_array(data, "saved_projectors");

		if (savedProjectors) {
			LoadSavedProjectors(savedProjectors);
			OpenSavedProjectors();
			activateWindow();
		}

		obs_data_array_release(savedProjectors);
	}

	/* ------------------- */

	std::string file_base = strrchr(file, '/') + 1;
	file_base.erase(file_base.size() - 5, 5);

	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollection",
			  name);
	config_set_string(App()->GlobalConfig(), "Basic", "SceneCollectionFile",
			  file_base.c_str());

	obs_data_array_t *quickTransitionData =
		obs_data_get_array(data, "quick_transitions");
	LoadQuickTransitions(quickTransitionData);
	obs_data_array_release(quickTransitionData);

	RefreshQuickTransitions();

	bool previewLocked = obs_data_get_bool(data, "preview_locked");
	ui->preview->SetLocked(previewLocked);
	//ui->actionLockPreview->setChecked(previewLocked);

	/* ---------------------- */

	bool fixedScaling = obs_data_get_bool(data, "scaling_enabled");
	int scalingLevel = (int)obs_data_get_int(data, "scaling_level");
	float scrollOffX = (float)obs_data_get_double(data, "scaling_off_x");
	float scrollOffY = (float)obs_data_get_double(data, "scaling_off_y");

	if (fixedScaling) {
		ui->preview->SetScalingLevel(scalingLevel);
		ui->preview->SetScrollingOffset(scrollOffX, scrollOffY);
	}
	ui->preview->SetFixedScaling(fixedScaling);
	emit ui->preview->DisplayResized();

	/* ---------------------- */

	if (api)
		api->on_load(modulesObj);

	obs_data_release(modulesObj);
	obs_data_release(data);

	if (!opt_starting_scene.empty())
		opt_starting_scene.clear();
#if 0
	if (opt_start_streaming) {
		blog(LOG_INFO, "Starting stream due to command line parameter");
		QMetaObject::invokeMethod(this, "StartStreaming",
					  Qt::QueuedConnection);
		opt_start_streaming = false;
	}
#endif
	if (opt_start_recording) {
		blog(LOG_INFO,
		     "Starting recording due to command line parameter");
		QMetaObject::invokeMethod(this, "StartRecording",
					  Qt::QueuedConnection);
		opt_start_recording = false;
	}

	if (opt_start_replaybuffer) {
		QMetaObject::invokeMethod(this, "StartReplayBuffer",
					  Qt::QueuedConnection);
		opt_start_replaybuffer = false;
	}

	copyStrings.clear();
	copyFiltersString = nullptr;

	LogScenes();

	disableSaving--;

	if (api) {
		api->on_event(OBS_FRONTEND_EVENT_SCENE_CHANGED);
		api->on_event(OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED);
	}
}

#define SERVICE_PATH "service.json"

void OBSBasic::SaveService()
{
	if (!service)
		return;

	char serviceJsonPath[512];
	int ret = GetProfilePath(serviceJsonPath, sizeof(serviceJsonPath),
				 SERVICE_PATH);
	if (ret <= 0)
		return;

	obs_data_t *data = obs_data_create();
	obs_data_t *settings = obs_service_get_settings(service);

	obs_data_set_string(data, "type", obs_service_get_type(service));
	obs_data_set_obj(data, "settings", settings);

	if (!obs_data_save_json_safe(data, serviceJsonPath, "tmp", "bak"))
		blog(LOG_WARNING, "Failed to save service");

	obs_data_release(settings);
	obs_data_release(data);
}

bool OBSBasic::LoadService()
{
	const char *type;

	char serviceJsonPath[512];
	int ret = GetProfilePath(serviceJsonPath, sizeof(serviceJsonPath),
				 SERVICE_PATH);
	if (ret <= 0)
		return false;

	obs_data_t *data =
		obs_data_create_from_json_file_safe(serviceJsonPath, "bak");

	obs_data_set_default_string(data, "type", "rtmp_common");
	type = obs_data_get_string(data, "type");

	obs_data_t *settings = obs_data_get_obj(data, "settings");
	obs_data_t *hotkey_data = obs_data_get_obj(data, "hotkeys");

	service = obs_service_create(type, "default_service", settings,
				     hotkey_data);
	obs_service_release(service);

	obs_data_release(hotkey_data);
	obs_data_release(settings);
	obs_data_release(data);

	return !!service;
}

bool OBSBasic::InitService()
{
	ProfileScope("OBSBasic::InitService");

	if (LoadService())
		return true;

	service = obs_service_create("rtmp_common", "default_service", nullptr,
				     nullptr);
	if (!service)
		return false;
	obs_service_release(service);

	return true;
}

static const double scaled_vals[] = {1.0,         1.25, (1.0 / 0.75), 1.5,
				     (1.0 / 0.6), 1.75, 2.0,          2.25,
				     2.5,         2.75, 3.0,          0.0};

extern void CheckExistingCookieId();

bool OBSBasic::InitBasicConfigDefaults()
{
	QList<QScreen *> screens = QGuiApplication::screens();

	if (!screens.size()) {
		OBSErrorBox(NULL, "There appears to be no monitors.  Er, this "
				  "technically shouldn't be possible.");
		return false;
	}

	QScreen *primaryScreen = QGuiApplication::primaryScreen();

	uint32_t cx = primaryScreen->size().width();
	uint32_t cy = primaryScreen->size().height();

	bool oldResolutionDefaults = config_get_bool(
		App()->GlobalConfig(), "General", "Pre19Defaults");

	/* use 1920x1080 for new default base res if main monitor is above
	 * 1920x1080, but don't apply for people from older builds -- only to
	 * new users */
	if (!oldResolutionDefaults && (cx * cy) > (1920 * 1080)) {
		cx = 1920;
		cy = 1080;
	}

	bool changed = false;

	/* ----------------------------------------------------- */
	/* move over old FFmpeg track settings                   */
	if (config_has_user_value(basicConfig, "AdvOut", "FFAudioTrack") &&
	    !config_has_user_value(basicConfig, "AdvOut", "Pre22.1Settings")) {

		int track = (int)config_get_int(basicConfig, "AdvOut",
						"FFAudioTrack");
		config_set_int(basicConfig, "AdvOut", "FFAudioMixes",
			       1LL << (track - 1));
		config_set_bool(basicConfig, "AdvOut", "Pre22.1Settings", true);
		changed = true;
	}

	/* ----------------------------------------------------- */
	/* move over mixer values in advanced if older config */
	if (config_has_user_value(basicConfig, "AdvOut", "RecTrackIndex") &&
	    !config_has_user_value(basicConfig, "AdvOut", "RecTracks")) {

		uint64_t track =
			config_get_uint(basicConfig, "AdvOut", "RecTrackIndex");
		track = 1ULL << (track - 1);
		config_set_uint(basicConfig, "AdvOut", "RecTracks", track);
		config_remove_value(basicConfig, "AdvOut", "RecTrackIndex");
		changed = true;
	}

	/* ----------------------------------------------------- */
	/* set twitch chat extensions to "both" if prev version  */
	/* is under 24.1                                         */
	if (config_get_bool(GetGlobalConfig(), "General", "Pre24.1Defaults") &&
	    !config_has_user_value(basicConfig, "Twitch", "AddonChoice")) {
		config_set_int(basicConfig, "Twitch", "AddonChoice", 3);
		changed = true;
	}

	/* ----------------------------------------------------- */

	if (changed)
		config_save_safe(basicConfig, "tmp", nullptr);

	/* ----------------------------------------------------- */

	config_set_default_string(basicConfig, "Output", "Mode", "Simple");

	config_set_default_string(basicConfig, "SimpleOutput", "FilePath",
				  GetDefaultVideoSavePath().c_str());
	config_set_default_string(basicConfig, "SimpleOutput", "RecFormat",
				  "mkv");
	config_set_default_uint(basicConfig, "SimpleOutput", "VBitrate", 2500);
	config_set_default_uint(basicConfig, "SimpleOutput", "ABitrate", 160);
	config_set_default_bool(basicConfig, "SimpleOutput", "UseAdvanced",
				false);
	config_set_default_bool(basicConfig, "SimpleOutput", "EnforceBitrate",
				true);
	config_set_default_string(basicConfig, "SimpleOutput", "Preset",
				  "veryfast");
	config_set_default_string(basicConfig, "SimpleOutput", "NVENCPreset",
				  "hq");
	config_set_default_string(basicConfig, "SimpleOutput", "RecQuality",
				  "Stream");
	config_set_default_bool(basicConfig, "SimpleOutput", "RecRB", false);
	config_set_default_int(basicConfig, "SimpleOutput", "RecRBTime", 20);
	config_set_default_int(basicConfig, "SimpleOutput", "RecRBSize", 512);
	config_set_default_string(basicConfig, "SimpleOutput", "RecRBPrefix",
				  "Replay");

	config_set_default_bool(basicConfig, "AdvOut", "ApplyServiceSettings",
				true);
	config_set_default_bool(basicConfig, "AdvOut", "UseRescale", false);
	config_set_default_uint(basicConfig, "AdvOut", "TrackIndex", 1);
	config_set_default_string(basicConfig, "AdvOut", "Encoder", "obs_x264");

	config_set_default_string(basicConfig, "AdvOut", "RecType", "Standard");

	config_set_default_string(basicConfig, "AdvOut", "RecFilePath",
				  GetDefaultVideoSavePath().c_str());
	config_set_default_string(basicConfig, "AdvOut", "RecFormat", "mkv");
	config_set_default_bool(basicConfig, "AdvOut", "RecUseRescale", false);
	config_set_default_uint(basicConfig, "AdvOut", "RecTracks", (1 << 0));
	config_set_default_string(basicConfig, "AdvOut", "RecEncoder", "none");
	config_set_default_uint(basicConfig, "AdvOut", "FLVTrack", 1);

	config_set_default_bool(basicConfig, "AdvOut", "FFOutputToFile", true);
	config_set_default_string(basicConfig, "AdvOut", "FFFilePath",
				  GetDefaultVideoSavePath().c_str());
	config_set_default_string(basicConfig, "AdvOut", "FFExtension", "mp4");
	config_set_default_uint(basicConfig, "AdvOut", "FFVBitrate", 2500);
	config_set_default_uint(basicConfig, "AdvOut", "FFVGOPSize", 250);
	config_set_default_bool(basicConfig, "AdvOut", "FFUseRescale", false);
	config_set_default_bool(basicConfig, "AdvOut", "FFIgnoreCompat", false);
	config_set_default_uint(basicConfig, "AdvOut", "FFABitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "FFAudioMixes", 1);

	config_set_default_uint(basicConfig, "AdvOut", "Track1Bitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "Track2Bitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "Track3Bitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "Track4Bitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "Track5Bitrate", 160);
	config_set_default_uint(basicConfig, "AdvOut", "Track6Bitrate", 160);

	config_set_default_bool(basicConfig, "AdvOut", "RecRB", false);
	config_set_default_uint(basicConfig, "AdvOut", "RecRBTime", 20);
	config_set_default_int(basicConfig, "AdvOut", "RecRBSize", 512);

	config_set_default_uint(basicConfig, "Video", "BaseCX", cx);
	config_set_default_uint(basicConfig, "Video", "BaseCY", cy);

	/* don't allow BaseCX/BaseCY to be susceptible to defaults changing */
	if (!config_has_user_value(basicConfig, "Video", "BaseCX") ||
	    !config_has_user_value(basicConfig, "Video", "BaseCY")) {
		config_set_uint(basicConfig, "Video", "BaseCX", cx);
		config_set_uint(basicConfig, "Video", "BaseCY", cy);
		config_save_safe(basicConfig, "tmp", nullptr);
	}

	config_set_default_string(basicConfig, "Output", "FilenameFormatting",
				  "%CCYY-%MM-%DD %hh-%mm-%ss");

	config_set_default_bool(basicConfig, "Output", "DelayEnable", false);
	config_set_default_uint(basicConfig, "Output", "DelaySec", 20);
	config_set_default_bool(basicConfig, "Output", "DelayPreserve", true);

	config_set_default_bool(basicConfig, "Output", "Reconnect", true);
	config_set_default_uint(basicConfig, "Output", "RetryDelay", 10);
	config_set_default_uint(basicConfig, "Output", "MaxRetries", 20);

	config_set_default_string(basicConfig, "Output", "BindIP", "default");
	config_set_default_bool(basicConfig, "Output", "NewSocketLoopEnable",
				false);
	config_set_default_bool(basicConfig, "Output", "LowLatencyEnable",
				false);

	int i = 0;
	uint32_t scale_cx = cx;
	uint32_t scale_cy = cy;

	/* use a default scaled resolution that has a pixel count no higher
	 * than 1280x720 */
	while (((scale_cx * scale_cy) > (1280 * 720)) && scaled_vals[i] > 0.0) {
		double scale = scaled_vals[i++];
		scale_cx = uint32_t(double(cx) / scale);
		scale_cy = uint32_t(double(cy) / scale);
	}

	config_set_default_uint(basicConfig, "Video", "OutputCX", scale_cx);
	config_set_default_uint(basicConfig, "Video", "OutputCY", scale_cy);

	/* don't allow OutputCX/OutputCY to be susceptible to defaults
	 * changing */
	if (!config_has_user_value(basicConfig, "Video", "OutputCX") ||
	    !config_has_user_value(basicConfig, "Video", "OutputCY")) {
		config_set_uint(basicConfig, "Video", "OutputCX", scale_cx);
		config_set_uint(basicConfig, "Video", "OutputCY", scale_cy);
		config_save_safe(basicConfig, "tmp", nullptr);
	}

	config_set_default_uint(basicConfig, "Video", "FPSType", 0);
	config_set_default_string(basicConfig, "Video", "FPSCommon", "30");
	config_set_default_uint(basicConfig, "Video", "FPSInt", 30);
	config_set_default_uint(basicConfig, "Video", "FPSNum", 30);
	config_set_default_uint(basicConfig, "Video", "FPSDen", 1);
	config_set_default_string(basicConfig, "Video", "ScaleType", "bicubic");
	config_set_default_string(basicConfig, "Video", "ColorFormat", "NV12");
	config_set_default_string(basicConfig, "Video", "ColorSpace", "601");
	config_set_default_string(basicConfig, "Video", "ColorRange",
				  "Partial");

	config_set_default_string(basicConfig, "Audio", "MonitoringDeviceId",
				  "default");
	config_set_default_string(
		basicConfig, "Audio", "MonitoringDeviceName",
		Str("Basic.Settings.Advanced.Audio.MonitoringDevice"
		    ".Default"));
	config_set_default_uint(basicConfig, "Audio", "SampleRate", 44100);
	config_set_default_string(basicConfig, "Audio", "ChannelSetup",
				  "Stereo");
	config_set_default_double(basicConfig, "Audio", "MeterDecayRate",
				  VOLUME_METER_DECAY_FAST);
	config_set_default_uint(basicConfig, "Audio", "PeakMeterType", 0);

	CheckExistingCookieId();

	return true;
}

extern bool EncoderAvailable(const char *encoder);

void OBSBasic::InitBasicConfigDefaults2()
{
	bool oldEncDefaults = config_get_bool(App()->GlobalConfig(), "General",
					      "Pre23Defaults");
	bool useNV = EncoderAvailable("ffmpeg_nvenc") && !oldEncDefaults;

	config_set_default_string(basicConfig, "SimpleOutput", "StreamEncoder",
				  useNV ? SIMPLE_ENCODER_NVENC
					: SIMPLE_ENCODER_X264);
	config_set_default_string(basicConfig, "SimpleOutput", "RecEncoder",
				  useNV ? SIMPLE_ENCODER_NVENC
					: SIMPLE_ENCODER_X264);
}

bool OBSBasic::InitBasicConfig()
{
	ProfileScope("OBSBasic::InitBasicConfig");

	char configPath[512];

	int ret = GetProfilePath(configPath, sizeof(configPath), "");
	if (ret <= 0) {
		OBSErrorBox(nullptr, "Failed to get profile path");
		return false;
	}

	if (os_mkdir(configPath) == MKDIR_ERROR) {
		OBSErrorBox(nullptr, "Failed to create profile path");
		return false;
	}

	ret = GetProfilePath(configPath, sizeof(configPath), "basic.ini");
	if (ret <= 0) {
		OBSErrorBox(nullptr, "Failed to get base.ini path");
		return false;
	}

	int code = basicConfig.Open(configPath, CONFIG_OPEN_ALWAYS);
	if (code != CONFIG_SUCCESS) {
		OBSErrorBox(NULL, "Failed to open basic.ini: %d", code);
		return false;
	}

	if (config_get_string(basicConfig, "General", "Name") == nullptr) {
		const char *curName = config_get_string(App()->GlobalConfig(),
							"Basic", "Profile");

		config_set_string(basicConfig, "General", "Name", curName);
		basicConfig.SaveSafe("tmp");
	}

	return InitBasicConfigDefaults();
}

void OBSBasic::InitOBSCallbacks()
{
	ProfileScope("OBSBasic::InitOBSCallbacks");

	signalHandlers.reserve(signalHandlers.size() + 7);
	signalHandlers.emplace_back(obs_get_signal_handler(), "source_create",
				    OBSBasic::SourceCreated, this);
	signalHandlers.emplace_back(obs_get_signal_handler(), "source_remove",
				    OBSBasic::SourceRemoved, this);
	signalHandlers.emplace_back(obs_get_signal_handler(), "source_activate",
				    OBSBasic::SourceActivated, this);
	signalHandlers.emplace_back(obs_get_signal_handler(),
				    "source_deactivate",
				    OBSBasic::SourceDeactivated, this);
	signalHandlers.emplace_back(obs_get_signal_handler(),
				    "source_audio_activate",
				    OBSBasic::SourceAudioActivated, this);
	signalHandlers.emplace_back(obs_get_signal_handler(),
				    "source_audio_deactivate",
				    OBSBasic::SourceAudioDeactivated, this);
	signalHandlers.emplace_back(obs_get_signal_handler(), "source_rename",
				    OBSBasic::SourceRenamed, this);
}

void OBSBasic::InitPrimitives()
{
	ProfileScope("OBSBasic::InitPrimitives");

	obs_enter_graphics();

	gs_render_start(true);
	gs_vertex2f(0.0f, 0.0f);
	gs_vertex2f(0.0f, 1.0f);
	gs_vertex2f(1.0f, 0.0f);
	gs_vertex2f(1.0f, 1.0f);
	box = gs_render_save();

	gs_render_start(true);
	gs_vertex2f(0.0f, 0.0f);
	gs_vertex2f(0.0f, 1.0f);
	boxLeft = gs_render_save();

	gs_render_start(true);
	gs_vertex2f(0.0f, 0.0f);
	gs_vertex2f(1.0f, 0.0f);
	boxTop = gs_render_save();

	gs_render_start(true);
	gs_vertex2f(1.0f, 0.0f);
	gs_vertex2f(1.0f, 1.0f);
	boxRight = gs_render_save();

	gs_render_start(true);
	gs_vertex2f(0.0f, 1.0f);
	gs_vertex2f(1.0f, 1.0f);
	boxBottom = gs_render_save();

	gs_render_start(true);
	for (int i = 0; i <= 360; i += (360 / 20)) {
		float pos = RAD(float(i));
		gs_vertex2f(cosf(pos), sinf(pos));
	}
	circle = gs_render_save();

	obs_leave_graphics();
}

void OBSBasic::ReplayBufferClicked()
{
	if (outputHandler->ReplayBufferActive())
		StopReplayBuffer();
	else
		StartReplayBuffer();
};

void OBSBasic::ResetOutputs()
{
	ProfileScope("OBSBasic::ResetOutputs");

	const char *mode = config_get_string(basicConfig, "Output", "Mode");
	bool advOut = astrcmpi(mode, "Advanced") == 0;

	if (!outputHandler || !outputHandler->Active()) {
		outputHandler.reset();
		outputHandler.reset(advOut ? CreateAdvancedOutputHandler(this)
					   : CreateSimpleOutputHandler(this));

		delete replayBufferButton;
		delete replayLayout;

		if (outputHandler->replayBuffer) {
			replayBufferButton = new ReplayBufferButton(
				QTStr("Basic.Main.StartReplayBuffer"), this);
			replayBufferButton->setCheckable(true);
			connect(replayBufferButton.data(),
				&QPushButton::clicked, this,
				&OBSBasic::ReplayBufferClicked);

			replayLayout = new QHBoxLayout(this);
			replayLayout->addWidget(replayBufferButton);

			replayBufferButton->setProperty("themeID",
							"replayBufferButton");
			/// ui->buttonsVLayout->insertLayout(2, replayLayout);
		}

		if (sysTrayReplayBuffer)
			sysTrayReplayBuffer->setEnabled(
				!!outputHandler->replayBuffer);
	} else {
		outputHandler->Update();
	}
}

static void AddProjectorMenuMonitors(QMenu *parent, QObject *target,
				     const char *slot);

#define STARTUP_SEPARATOR \
	"==== Startup complete ==============================================="
#define SHUTDOWN_SEPARATOR \
	"==== Shutting down =================================================="

#define UNSUPPORTED_ERROR                                                     \
	"Failed to initialize video:\n\nRequired graphics API functionality " \
	"not found.  Your GPU may not be supported."

#define UNKNOWN_ERROR                                                  \
	"Failed to initialize video.  Your GPU may not be supported, " \
	"or your graphics drivers may need to be updated."

void OBSBasic::OBSInit()
{
	ProfileScope("OBSBasic::OBSInit");

	const char *sceneCollection = config_get_string(
		App()->GlobalConfig(), "Basic", "SceneCollectionFile");
	char savePath[512];
	char fileName[512];
	int ret;

	if (!sceneCollection)
		throw "Failed to get scene collection name";

	ret = snprintf(fileName, 512, "obs-studio/basic/scenes/%s.json",
		       sceneCollection);
	if (ret <= 0)
		throw "Failed to create scene collection file name";

	ret = GetConfigPath(savePath, sizeof(savePath), fileName);
	if (ret <= 0)
		throw "Failed to get scene collection json file path";

	if (!InitBasicConfig())
		throw "Failed to load basic.ini";
	if (!ResetAudio())
		throw "Failed to initialize audio";

	ret = ResetVideo();

	switch (ret) {
	case OBS_VIDEO_MODULE_NOT_FOUND:
		throw "Failed to initialize video:  Graphics module not found";
	case OBS_VIDEO_NOT_SUPPORTED:
		throw UNSUPPORTED_ERROR;
	case OBS_VIDEO_INVALID_PARAM:
		throw "Failed to initialize video:  Invalid parameters";
	default:
		if (ret != OBS_VIDEO_SUCCESS)
			throw UNKNOWN_ERROR;
	}

	/* load audio monitoring */
#if defined(_WIN32) || defined(__APPLE__) || HAVE_PULSEAUDIO
	const char *device_name =
		config_get_string(basicConfig, "Audio", "MonitoringDeviceName");
	const char *device_id =
		config_get_string(basicConfig, "Audio", "MonitoringDeviceId");

	obs_set_audio_monitoring_device(device_name, device_id);

	blog(LOG_INFO, "Audio monitoring device:\n\tname: %s\n\tid: %s",
	     device_name, device_id);
#endif

	InitOBSCallbacks();
	InitHotkeys();

	AddExtraModulePaths();
	blog(LOG_INFO, "---------------------------------");
	obs_load_all_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_log_loaded_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_post_load_modules();

#ifdef BROWSER_AVAILABLE
	cef = obs_browser_init_panel();
#endif

	InitBasicConfigDefaults2();

	CheckForSimpleModeX264Fallback();

	blog(LOG_INFO, STARTUP_SEPARATOR);

	ResetOutputs();
	CreateHotkeys();

	if (!InitService())
		throw "Failed to initialize service";

	InitPrimitives();

	sceneDuplicationMode = config_get_bool(
		App()->GlobalConfig(), "BasicWindow", "SceneDuplicationMode");
	swapScenesMode = config_get_bool(App()->GlobalConfig(), "BasicWindow",
					 "SwapScenesMode");
	editPropertiesMode = config_get_bool(
		App()->GlobalConfig(), "BasicWindow", "EditPropertiesMode");

#define SET_VISIBILITY(name, control)                                         \
	do {                                                                  \
		if (config_has_user_value(App()->GlobalConfig(),              \
					  "BasicWindow", name)) {             \
			bool visible = config_get_bool(App()->GlobalConfig(), \
						       "BasicWindow", name);  \
			ui->control->setChecked(visible);                     \
		}                                                             \
	} while (false)

	//SET_VISIBILITY("ShowListboxToolbars", toggleListboxToolbars);
	//SET_VISIBILITY("ShowStatusBar", toggleStatusBar);
#undef SET_VISIBILITY

	bool sourceIconsVisible = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "ShowSourceIcons");
	//ui->toggleSourceIcons->setChecked(sourceIconsVisible);

	{
		ProfileScope("OBSBasic::Load");
		disableSaving--;
		Load(savePath);
        SetControlProperties();// storm
		disableSaving++;
	}

	TimedCheckForUpdates();
	loaded = true;

	previewEnabled = config_get_bool(App()->GlobalConfig(), "BasicWindow",
					 "PreviewEnabled");

	if (!previewEnabled && !IsPreviewProgramMode())
		QMetaObject::invokeMethod(this, "EnablePreviewDisplay",
					  Qt::QueuedConnection,
					  Q_ARG(bool, previewEnabled));

#ifdef _WIN32
	uint32_t winVer = GetWindowsVersion();
	if (winVer > 0 && winVer < 0x602) {
		bool disableAero =
			config_get_bool(basicConfig, "Video", "DisableAero");
		SetAeroEnabled(!disableAero);
	}
#endif

	RefreshSceneCollections();
	RefreshProfiles();
	disableSaving--;

	auto addDisplay = [this](OBSQTDisplay *window) {
		obs_display_add_draw_callback(window->GetDisplay(),
					      OBSBasic::RenderMain, this);

		struct obs_video_info ovi;
		if (obs_get_video_info(&ovi))
			ResizePreview(ovi.base_width, ovi.base_height);
	};

	connect(ui->preview, &OBSQTDisplay::DisplayCreated, addDisplay);

#ifdef _WIN32
	SetWin32DropStyle(this);
	show();
#endif

	bool alwaysOnTop = config_get_bool(App()->GlobalConfig(), "BasicWindow",
					   "AlwaysOnTop");
	if (alwaysOnTop || opt_always_on_top) {
		SetAlwaysOnTop(this, true);
		// ui->actionAlwaysOnTop->setChecked(true);
	}

#ifndef _WIN32
	show();
#endif
#if 0
	/* setup stats dock */
	OBSBasicStats *statsDlg = new OBSBasicStats(statsDock, false);
	statsDock->setWidget(statsDlg);
#endif
	/* ----------------------------- */
	/* add custom browser docks      */

#ifdef BROWSER_AVAILABLE
	if (cef) {
		QAction *action = new QAction(QTStr("Basic.MainMenu."
						    "View.Docks."
						    "CustomBrowserDocks"),
					      this);
		//ui->viewMenuDocks->insertAction(ui->toggleScenes, action);
		connect(action, &QAction::triggered, this,
			&OBSBasic::ManageExtraBrowserDocks);
		//ui->viewMenuDocks->insertSeparator(ui->toggleScenes);

		LoadExtraBrowserDocks();
	}
#endif

	const char *dockStateStr = config_get_string(
		App()->GlobalConfig(), "BasicWindow", "DockState");
	if (!dockStateStr) {
		on_resetUI_triggered();
	} else {
		QByteArray dockState =
			QByteArray::fromBase64(QByteArray(dockStateStr));
		//if (!restoreState(dockState))
		//	on_resetUI_triggered();
	}

	bool pre23Defaults = config_get_bool(App()->GlobalConfig(), "General",
					     "Pre23Defaults");
	if (pre23Defaults) {
		bool resetDockLock23 = config_get_bool(
			App()->GlobalConfig(), "General", "ResetDockLock23");
		if (!resetDockLock23) {
			config_set_bool(App()->GlobalConfig(), "General",
					"ResetDockLock23", true);
			config_remove_value(App()->GlobalConfig(),
					    "BasicWindow", "DocksLocked");
			config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
		}
	}

	bool docksLocked = config_get_bool(App()->GlobalConfig(), "BasicWindow",
					   "DocksLocked");
	on_lockUI_toggled(docksLocked);
#if 0
	ui->lockUI->blockSignals(true);
	ui->lockUI->setChecked(docksLocked);
	ui->lockUI->blockSignals(false);
#endif
#ifndef __APPLE__
	SystemTray(true);
#endif

	bool has_last_version = config_has_user_value(App()->GlobalConfig(),
						      "General", "LastVersion");
	bool first_run =
		config_get_bool(App()->GlobalConfig(), "General", "FirstRun");

	if (!first_run) {
		config_set_bool(App()->GlobalConfig(), "General", "FirstRun",
				true);
		config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
	}

	if (!first_run && !has_last_version && !Active()) {
		QString msg;
		msg = QTStr("Basic.FirstStartup.RunWizard");

		QMessageBox::StandardButton button = OBSMessageBox::question(
			this, QTStr("Basic.AutoConfig"), msg);

		if (button == QMessageBox::Yes) {
			QMetaObject::invokeMethod(this,
						  "on_autoConfigure_triggered",
						  Qt::QueuedConnection);
		} else {
			msg = QTStr("Basic.FirstStartup.RunWizard.NoClicked");
			OBSMessageBox::information(
				this, QTStr("Basic.AutoConfig"), msg);
		}
	}

	ToggleMixerLayout(config_get_bool(App()->GlobalConfig(), "BasicWindow",
					  "VerticalVolControl"));

	if (config_get_bool(basicConfig, "General", "OpenStatsOnStartup"))
		on_stats_triggered();

	OBSBasicStats::InitializeValues();

	/* ----------------------- */
	/* Add multiview menu      */
#if 0
	ui->viewMenu->addSeparator();

	multiviewProjectorMenu = new QMenu(QTStr("MultiviewProjector"));
	ui->viewMenu->addMenu(multiviewProjectorMenu);
	AddProjectorMenuMonitors(multiviewProjectorMenu, this,
				 SLOT(OpenMultiviewProjector()));
	connect(ui->viewMenu->menuAction(), &QAction::hovered, this,
		&OBSBasic::UpdateMultiviewProjectorMenu);
	ui->viewMenu->addAction(QTStr("MultiviewWindowed"), this,
				SLOT(OpenMultiviewWindow()));
#endif
	ui->sources->UpdateIcons();

	if (!opt_studio_mode) {
		SetPreviewProgramMode(config_get_bool(App()->GlobalConfig(),
						      "BasicWindow",
						      "PreviewProgramMode"));
	} else {
		SetPreviewProgramMode(true);
		opt_studio_mode = false;
	}

#if !defined(_WIN32) && !defined(__APPLE__)
#if 0
	delete ui->actionShowCrashLogs;
	delete ui->actionUploadLastCrashLog;
	delete ui->menuCrashLogs;
	delete ui->actionCheckForUpdates;
	ui->actionShowCrashLogs = nullptr;
	ui->actionUploadLastCrashLog = nullptr;
	ui->menuCrashLogs = nullptr;
	ui->actionCheckForUpdates = nullptr;
#endif
#endif

	OnFirstLoad();

#ifdef __APPLE__
	QMetaObject::invokeMethod(this, "DeferredSysTrayLoad",
				  Qt::QueuedConnection, Q_ARG(int, 10));
#endif
}

void OBSBasic::OnFirstLoad()
{
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_FINISHED_LOADING);

#if defined(BROWSER_AVAILABLE) && defined(_WIN32)
	/* Attempt to load init screen if available */
	if (cef) {
		WhatsNewInfoThread *wnit = new WhatsNewInfoThread();
		if (wnit) {
			connect(wnit, &WhatsNewInfoThread::Result, this,
				&OBSBasic::ReceivedIntroJson);
		}
		if (wnit) {
			introCheckThread.reset(wnit);
			introCheckThread->start();
		}
	}
#endif

	Auth::Load();
}

void OBSBasic::DeferredSysTrayLoad(int requeueCount)
{
	if (--requeueCount > 0) {
		QMetaObject::invokeMethod(this, "DeferredSysTrayLoad",
					  Qt::QueuedConnection,
					  Q_ARG(int, requeueCount));
		return;
	}

	/* Minimizng to tray on initial startup does not work on mac
	 * unless it is done in the deferred load */
	SystemTray(true);
}

/* shows a "what's new" page on startup of new versions using CEF */
void OBSBasic::ReceivedIntroJson(const QString &text)
{
#ifdef BROWSER_AVAILABLE
#ifdef _WIN32
	if (closing)
		return;

	std::string err;
	Json json = Json::parse(QT_TO_UTF8(text), err);
	if (!err.empty())
		return;

	std::string info_url;
	int info_increment = -1;

	/* check to see if there's an info page for this version */
	const Json::array &items = json.array_items();
	for (const Json &item : items) {
		const std::string &version = item["version"].string_value();
		const std::string &url = item["url"].string_value();
		int increment = item["increment"].int_value();
		int rc = item["RC"].int_value();

		int major = 0;
		int minor = 0;

		sscanf(version.c_str(), "%d.%d", &major, &minor);
#if OBS_RELEASE_CANDIDATE > 0
		if (major == OBS_RELEASE_CANDIDATE_MAJOR &&
		    minor == OBS_RELEASE_CANDIDATE_MINOR &&
		    rc == OBS_RELEASE_CANDIDATE) {
#else
		if (major == LIBOBS_API_MAJOR_VER &&
		    minor == LIBOBS_API_MINOR_VER && rc == 0) {
#endif
			info_url = url;
			info_increment = increment;
		}
	}

	/* this version was not found, or no info for this version */
	if (info_increment == -1) {
		return;
	}

#if OBS_RELEASE_CANDIDATE > 0
	uint32_t lastVersion = config_get_int(App()->GlobalConfig(), "General",
					      "LastRCVersion");
#else
	uint32_t lastVersion =
		config_get_int(App()->GlobalConfig(), "General", "LastVersion");
#endif

	int current_version_increment = -1;

#if OBS_RELEASE_CANDIDATE > 0
	if (lastVersion < OBS_RELEASE_CANDIDATE_VER) {
#else
	if ((lastVersion & ~0xFFFF) < (LIBOBS_API_VER & ~0xFFFF)) {
#endif
		config_set_int(App()->GlobalConfig(), "General",
			       "InfoIncrement", -1);
	} else {
		current_version_increment = config_get_int(
			App()->GlobalConfig(), "General", "InfoIncrement");
	}

	if (info_increment <= current_version_increment) {
		return;
	}

	config_set_int(App()->GlobalConfig(), "General", "InfoIncrement",
		       info_increment);

	/* Don't show What's New dialog for new users */
#if !defined(OBS_RELEASE_CANDIDATE) || OBS_RELEASE_CANDIDATE == 0
	if (!lastVersion) {
		return;
	}
#endif
	cef->init_browser();

	WhatsNewBrowserInitThread *wnbit =
		new WhatsNewBrowserInitThread(QT_UTF8(info_url.c_str()));
	if (wnbit) {
		connect(wnbit, &WhatsNewBrowserInitThread::Result, this,
			&OBSBasic::ShowWhatsNew);
	}
	if (wnbit) {
		whatsNewInitThread.reset(wnbit);
		whatsNewInitThread->start();
	}
#else
	UNUSED_PARAMETER(text);
#endif
#else
	UNUSED_PARAMETER(text);
#endif
}

void OBSBasic::ShowWhatsNew(const QString &url)
{
#ifdef BROWSER_AVAILABLE
#ifdef _WIN32
	if (closing)
		return;

	std::string info_url = QT_TO_UTF8(url);

	QDialog *dlg = new QDialog(this);
	dlg->setAttribute(Qt::WA_DeleteOnClose, true);
	dlg->setWindowTitle("What's New");
	dlg->resize(700, 600);

	Qt::WindowFlags flags = dlg->windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	dlg->setWindowFlags(flags & (~helpFlag));

	QCefWidget *cefWidget = cef->create_widget(nullptr, info_url);
	if (!cefWidget) {
		return;
	}

	connect(cefWidget, SIGNAL(titleChanged(const QString &)), dlg,
		SLOT(setWindowTitle(const QString &)));

	QPushButton *close = new QPushButton(QTStr("Close"));
	connect(close, &QAbstractButton::clicked, dlg, &QDialog::accept);

	QHBoxLayout *bottomLayout = new QHBoxLayout();
	bottomLayout->addStretch();
	bottomLayout->addWidget(close);
	bottomLayout->addStretch();

	QVBoxLayout *topLayout = new QVBoxLayout(dlg);
	topLayout->addWidget(cefWidget);
	topLayout->addLayout(bottomLayout);

	dlg->show();
#else
	UNUSED_PARAMETER(url);
#endif
#else
	UNUSED_PARAMETER(url);
#endif
}

void OBSBasic::UpdateMultiviewProjectorMenu()
{
	multiviewProjectorMenu->clear();
	AddProjectorMenuMonitors(multiviewProjectorMenu, this,
				 SLOT(OpenMultiviewProjector()));
}

void OBSBasic::InitHotkeys()
{
	ProfileScope("OBSBasic::InitHotkeys");

	struct obs_hotkeys_translations t = {};
	t.insert = Str("Hotkeys.Insert");
	t.del = Str("Hotkeys.Delete");
	t.home = Str("Hotkeys.Home");
	t.end = Str("Hotkeys.End");
	t.page_up = Str("Hotkeys.PageUp");
	t.page_down = Str("Hotkeys.PageDown");
	t.num_lock = Str("Hotkeys.NumLock");
	t.scroll_lock = Str("Hotkeys.ScrollLock");
	t.caps_lock = Str("Hotkeys.CapsLock");
	t.backspace = Str("Hotkeys.Backspace");
	t.tab = Str("Hotkeys.Tab");
	t.print = Str("Hotkeys.Print");
	t.pause = Str("Hotkeys.Pause");
	t.left = Str("Hotkeys.Left");
	t.right = Str("Hotkeys.Right");
	t.up = Str("Hotkeys.Up");
	t.down = Str("Hotkeys.Down");
#ifdef _WIN32
	t.meta = Str("Hotkeys.Windows");
#else
	t.meta = Str("Hotkeys.Super");
#endif
	t.menu = Str("Hotkeys.Menu");
	t.space = Str("Hotkeys.Space");
	t.numpad_num = Str("Hotkeys.NumpadNum");
	t.numpad_multiply = Str("Hotkeys.NumpadMultiply");
	t.numpad_divide = Str("Hotkeys.NumpadDivide");
	t.numpad_plus = Str("Hotkeys.NumpadAdd");
	t.numpad_minus = Str("Hotkeys.NumpadSubtract");
	t.numpad_decimal = Str("Hotkeys.NumpadDecimal");
	t.apple_keypad_num = Str("Hotkeys.AppleKeypadNum");
	t.apple_keypad_multiply = Str("Hotkeys.AppleKeypadMultiply");
	t.apple_keypad_divide = Str("Hotkeys.AppleKeypadDivide");
	t.apple_keypad_plus = Str("Hotkeys.AppleKeypadAdd");
	t.apple_keypad_minus = Str("Hotkeys.AppleKeypadSubtract");
	t.apple_keypad_decimal = Str("Hotkeys.AppleKeypadDecimal");
	t.apple_keypad_equal = Str("Hotkeys.AppleKeypadEqual");
	t.mouse_num = Str("Hotkeys.MouseButton");
	t.escape = Str("Hotkeys.Escape");
	obs_hotkeys_set_translations(&t);

	obs_hotkeys_set_audio_hotkeys_translations(Str("Mute"), Str("Unmute"),
						   Str("Push-to-mute"),
						   Str("Push-to-talk"));

	obs_hotkeys_set_sceneitem_hotkeys_translations(Str("SceneItemShow"),
						       Str("SceneItemHide"));

	obs_hotkey_enable_callback_rerouting(true);
	obs_hotkey_set_callback_routing_func(OBSBasic::HotkeyTriggered, this);
}

void OBSBasic::ProcessHotkey(obs_hotkey_id id, bool pressed)
{
	obs_hotkey_trigger_routed_callback(id, pressed);
}

void OBSBasic::HotkeyTriggered(void *data, obs_hotkey_id id, bool pressed)
{
	OBSBasic &basic = *static_cast<OBSBasic *>(data);
	QMetaObject::invokeMethod(&basic, "ProcessHotkey",
				  Q_ARG(obs_hotkey_id, id),
				  Q_ARG(bool, pressed));
}

void OBSBasic::CreateHotkeys()
{
	ProfileScope("OBSBasic::CreateHotkeys");

	auto LoadHotkeyData = [&](const char *name) -> OBSData {
		const char *info =
			config_get_string(basicConfig, "Hotkeys", name);
		if (!info)
			return {};

		obs_data_t *data = obs_data_create_from_json(info);
		if (!data)
			return {};

		OBSData res = data;
		obs_data_release(data);
		return res;
	};

	auto LoadHotkey = [&](obs_hotkey_id id, const char *name) {
		obs_data_array_t *array =
			obs_data_get_array(LoadHotkeyData(name), "bindings");

		obs_hotkey_load(id, array);
		obs_data_array_release(array);
	};

	auto LoadHotkeyPair = [&](obs_hotkey_pair_id id, const char *name0,
				  const char *name1) {
		obs_data_array_t *array0 =
			obs_data_get_array(LoadHotkeyData(name0), "bindings");
		obs_data_array_t *array1 =
			obs_data_get_array(LoadHotkeyData(name1), "bindings");

		obs_hotkey_pair_load(id, array0, array1);
		obs_data_array_release(array0);
		obs_data_array_release(array1);
	};

#define MAKE_CALLBACK(pred, method, log_action)                            \
	[](void *data, obs_hotkey_pair_id, obs_hotkey_t *, bool pressed) { \
		OBSBasic &basic = *static_cast<OBSBasic *>(data);          \
		if ((pred) && pressed) {                                   \
			blog(LOG_INFO, log_action " due to hotkey");       \
			method();                                          \
			return true;                                       \
		}                                                          \
		return false;                                              \
	}
#if 0
	streamingHotkeys = obs_hotkey_pair_register_frontend(
		"OBSBasic.StartStreaming", Str("Basic.Main.StartStreaming"),
		"OBSBasic.StopStreaming", Str("Basic.Main.StopStreaming"),
		MAKE_CALLBACK(!basic.outputHandler->StreamingActive() &&
				      basic.ui->streamButton->isEnabled(),
			      basic.StartStreaming, "Starting stream"),
		MAKE_CALLBACK(basic.outputHandler->StreamingActive() &&
				      basic.ui->streamButton->isEnabled(),
			      basic.StopStreaming, "Stopping stream"),
		this, this);
	LoadHotkeyPair(streamingHotkeys, "OBSBasic.StartStreaming",
		       "OBSBasic.StopStreaming");

	auto cb = [](void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed) {
		OBSBasic &basic = *static_cast<OBSBasic *>(data);
		if (basic.outputHandler->StreamingActive() && pressed) {
			basic.ForceStopStreaming();
		}
	};

	forceStreamingStopHotkey = obs_hotkey_register_frontend(
		"OBSBasic.ForceStopStreaming",
		Str("Basic.Main.ForceStopStreaming"), cb, this);
	LoadHotkey(forceStreamingStopHotkey, "OBSBasic.ForceStopStreaming");
#endif
	recordingHotkeys = obs_hotkey_pair_register_frontend(
		"OBSBasic.StartRecording", Str("Basic.Main.StartRecording"),
		"OBSBasic.StopRecording", Str("Basic.Main.StopRecording"),
		MAKE_CALLBACK(!basic.outputHandler->RecordingActive() &&
				      !basic.ui->recordButton->isChecked(),
			      basic.StartRecording, "Starting recording"),
		MAKE_CALLBACK(basic.outputHandler->RecordingActive() &&
				      basic.ui->recordButton->isChecked(),
			      basic.StopRecording, "Stopping recording"),
		this, this);
	LoadHotkeyPair(recordingHotkeys, "OBSBasic.StartRecording",
		       "OBSBasic.StopRecording");

	pauseHotkeys = obs_hotkey_pair_register_frontend(
		"OBSBasic.PauseRecording", Str("Basic.Main.PauseRecording"),
		"OBSBasic.UnpauseRecording", Str("Basic.Main.UnpauseRecording"),
		MAKE_CALLBACK(basic.pause && !basic.pause->isChecked(),
			      basic.PauseRecording, "Pausing recording"),
		MAKE_CALLBACK(basic.pause && basic.pause->isChecked(),
			      basic.UnpauseRecording, "Unpausing recording"),
		this, this);
	LoadHotkeyPair(pauseHotkeys, "OBSBasic.PauseRecording",
		       "OBSBasic.UnpauseRecording");

	replayBufHotkeys = obs_hotkey_pair_register_frontend(
		"OBSBasic.StartReplayBuffer",
		Str("Basic.Main.StartReplayBuffer"),
		"OBSBasic.StopReplayBuffer", Str("Basic.Main.StopReplayBuffer"),
		MAKE_CALLBACK(!basic.outputHandler->ReplayBufferActive(),
			      basic.StartReplayBuffer,
			      "Starting replay buffer"),
		MAKE_CALLBACK(basic.outputHandler->ReplayBufferActive(),
			      basic.StopReplayBuffer, "Stopping replay buffer"),
		this, this);
	LoadHotkeyPair(replayBufHotkeys, "OBSBasic.StartReplayBuffer",
		       "OBSBasic.StopReplayBuffer");

	togglePreviewHotkeys = obs_hotkey_pair_register_frontend(
		"OBSBasic.EnablePreview",
		Str("Basic.Main.PreviewConextMenu.Enable"),
		"OBSBasic.DisablePreview", Str("Basic.Main.Preview.Disable"),
		MAKE_CALLBACK(!basic.previewEnabled, basic.EnablePreview,
			      "Enabling preview"),
		MAKE_CALLBACK(basic.previewEnabled, basic.DisablePreview,
			      "Disabling preview"),
		this, this);
	LoadHotkeyPair(togglePreviewHotkeys, "OBSBasic.EnablePreview",
		       "OBSBasic.DisablePreview");
#undef MAKE_CALLBACK

	auto togglePreviewProgram = [](void *data, obs_hotkey_id,
				       obs_hotkey_t *, bool pressed) {
		if (pressed)
			QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
						  "on_modeSwitch_clicked",
						  Qt::QueuedConnection);
	};

	togglePreviewProgramHotkey = obs_hotkey_register_frontend(
		"OBSBasic.TogglePreviewProgram",
		Str("Basic.TogglePreviewProgramMode"), togglePreviewProgram,
		this);
	LoadHotkey(togglePreviewProgramHotkey, "OBSBasic.TogglePreviewProgram");

	auto transition = [](void *data, obs_hotkey_id, obs_hotkey_t *,
			     bool pressed) {
		if (pressed)
			QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
						  "TransitionClicked",
						  Qt::QueuedConnection);
	};

	transitionHotkey = obs_hotkey_register_frontend(
		"OBSBasic.Transition", Str("Transition"), transition, this);
	LoadHotkey(transitionHotkey, "OBSBasic.Transition");

	auto resetStats = [](void *data, obs_hotkey_id, obs_hotkey_t *,
			     bool pressed) {
		if (pressed)
			QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
						  "ResetStatsHotkey",
						  Qt::QueuedConnection);
	};

	statsHotkey = obs_hotkey_register_frontend(
		"OBSBasic.ResetStats", Str("Basic.Stats.ResetStats"),
		resetStats, this);
	LoadHotkey(statsHotkey, "OBSBasic.ResetStats");
}

void OBSBasic::ClearHotkeys()
{
	//obs_hotkey_pair_unregister(streamingHotkeys);
	obs_hotkey_pair_unregister(recordingHotkeys);
	obs_hotkey_pair_unregister(pauseHotkeys);
	obs_hotkey_pair_unregister(replayBufHotkeys);
	obs_hotkey_pair_unregister(togglePreviewHotkeys);
	//obs_hotkey_unregister(forceStreamingStopHotkey);
	obs_hotkey_unregister(togglePreviewProgramHotkey);
	obs_hotkey_unregister(transitionHotkey);
	obs_hotkey_unregister(statsHotkey);
}

OBSBasic::~OBSBasic()
{
	/* clear out UI event queue */
	QApplication::sendPostedEvents(App());

	if (updateCheckThread && updateCheckThread->isRunning())
		updateCheckThread->wait();

	delete multiviewProjectorMenu;
	delete previewProjector;
	delete studioProgramProjector;
	delete previewProjectorSource;
	delete previewProjectorMain;
	delete sourceProjector;
	delete sceneProjectorMenu;
	delete scaleFilteringMenu;
	delete colorMenu;
	delete colorWidgetAction;
	delete colorSelect;
	delete deinterlaceMenu;
	delete perSceneTransitionMenu;
	delete shortcutFilter;
	delete trayMenu;
	delete programOptions;
	delete program;

	/* XXX: any obs data must be released before calling obs_shutdown.
	 * currently, we can't automate this with C++ RAII because of the
	 * delicate nature of obs_shutdown needing to be freed before the UI
	 * can be freed, and we have no control over the destruction order of
	 * the Qt UI stuff, so we have to manually clear any references to
	 * libobs. */
	// delete cpuUsageTimer;
	// os_cpu_usage_info_destroy(cpuUsageInfo);

	obs_hotkey_set_callback_routing_func(nullptr, nullptr);
	ClearHotkeys();

	service = nullptr;
	outputHandler.reset();

	if (interaction)
		delete interaction;

	if (properties)
		delete properties;

	if (filters)
		delete filters;

	if (transformWindow)
		delete transformWindow;

	if (advAudioWindow)
		delete advAudioWindow;

	if (about)
		delete about;

	obs_display_remove_draw_callback(ui->preview->GetDisplay(),
					 OBSBasic::RenderMain, this);

	obs_enter_graphics();
	gs_vertexbuffer_destroy(box);
	gs_vertexbuffer_destroy(boxLeft);
	gs_vertexbuffer_destroy(boxTop);
	gs_vertexbuffer_destroy(boxRight);
	gs_vertexbuffer_destroy(boxBottom);
	gs_vertexbuffer_destroy(circle);
	obs_leave_graphics();

	/* When shutting down, sometimes source references can get in to the
	 * event queue, and if we don't forcibly process those events they
	 * won't get processed until after obs_shutdown has been called.  I
	 * really wish there were a more elegant way to deal with this via C++,
	 * but Qt doesn't use C++ in a normal way, so you can't really rely on
	 * normal C++ behavior for your data to be freed in the order that you
	 * expect or want it to. */
	QApplication::sendPostedEvents(this);

	config_set_int(App()->GlobalConfig(), "General", "LastVersion",
		       LIBOBS_API_VER);
#if OBS_RELEASE_CANDIDATE > 0
	config_set_int(App()->GlobalConfig(), "General", "LastRCVersion",
		       OBS_RELEASE_CANDIDATE_VER);
#endif

	bool alwaysOnTop = IsAlwaysOnTop(this);

	config_set_bool(App()->GlobalConfig(), "BasicWindow", "PreviewEnabled",
			previewEnabled);
	config_set_bool(App()->GlobalConfig(), "BasicWindow", "AlwaysOnTop",
			alwaysOnTop);
	config_set_bool(App()->GlobalConfig(), "BasicWindow",
			"SceneDuplicationMode", sceneDuplicationMode);
	config_set_bool(App()->GlobalConfig(), "BasicWindow", "SwapScenesMode",
			swapScenesMode);
	config_set_bool(App()->GlobalConfig(), "BasicWindow",
			"EditPropertiesMode", editPropertiesMode);
	config_set_bool(App()->GlobalConfig(), "BasicWindow",
			"PreviewProgramMode", IsPreviewProgramMode());
#if 0
	config_set_bool(App()->GlobalConfig(), "BasicWindow", "DocksLocked",
			ui->lockUI->isChecked());
#endif
	config_save_safe(App()->GlobalConfig(), "tmp", nullptr);

#ifdef _WIN32
	uint32_t winVer = GetWindowsVersion();
	if (winVer > 0 && winVer < 0x602) {
		bool disableAero =
			config_get_bool(basicConfig, "Video", "DisableAero");
		if (disableAero) {
			SetAeroEnabled(true);
		}
	}
#endif

#ifdef BROWSER_AVAILABLE
	DestroyPanelCookieManager();
	delete cef;
	cef = nullptr;
#endif
}

void OBSBasic::SaveProjectNow()
{
	if (disableSaving)
		return;

	projectChanged = true;
	SaveProjectDeferred();
}

void OBSBasic::SaveProject()
{
	if (disableSaving)
		return;

	projectChanged = true;
	QMetaObject::invokeMethod(this, "SaveProjectDeferred",
				  Qt::QueuedConnection);
}

void OBSBasic::SaveProjectDeferred()
{
	if (disableSaving)
		return;

	if (!projectChanged)
		return;

	projectChanged = false;

	const char *sceneCollection = config_get_string(
		App()->GlobalConfig(), "Basic", "SceneCollectionFile");
	char savePath[512];
	char fileName[512];
	int ret;

	if (!sceneCollection)
		return;

	ret = snprintf(fileName, 512, "obs-studio/basic/scenes/%s.json",
		       sceneCollection);
	if (ret <= 0)
		return;

	ret = GetConfigPath(savePath, sizeof(savePath), fileName);
	if (ret <= 0)
		return;

	Save(savePath);
}

OBSSource OBSBasic::GetProgramSource()
{
	return OBSGetStrongRef(programScene);
}

OBSScene OBSBasic::GetCurrentScene()
{
	QListWidgetItem *item = ui->scenes->currentItem();
	return item ? GetOBSRef<OBSScene>(item) : nullptr;
}

OBSSceneItem OBSBasic::GetSceneItem(QListWidgetItem *item)
{
	return item ? GetOBSRef<OBSSceneItem>(item) : nullptr;
}

OBSSceneItem OBSBasic::GetCurrentSceneItem()
{
	return ui->sources->Get(GetTopSelectedSourceItem());
}

void OBSBasic::UpdatePreviewScalingMenu()
{
	bool fixedScaling = ui->preview->IsFixedScaling();
	float scalingAmount = ui->preview->GetScalingAmount();
	if (!fixedScaling) {
#if 0
		ui->actionScaleWindow->setChecked(true);
		ui->actionScaleCanvas->setChecked(false);
		ui->actionScaleOutput->setChecked(false);
#endif
		return;
	}

	obs_video_info ovi;
	obs_get_video_info(&ovi);
#if 0
	ui->actionScaleWindow->setChecked(false);
	ui->actionScaleCanvas->setChecked(scalingAmount == 1.0f);
	ui->actionScaleOutput->setChecked(scalingAmount ==
					  float(ovi.output_width) /
						  float(ovi.base_width));
#endif
}

void OBSBasic::CreateInteractionWindow(obs_source_t *source)
{
	if (interaction)
		interaction->close();

	interaction = new OBSBasicInteraction(this, source);
	interaction->Init();
	interaction->setAttribute(Qt::WA_DeleteOnClose, true);
}

void OBSBasic::CreatePropertiesWindow(obs_source_t *source)
{
	if (properties)
		properties->close();

	properties = new OBSBasicProperties(this, source);
	properties->Init();
	properties->setAttribute(Qt::WA_DeleteOnClose, true);
}

void OBSBasic::CreateFiltersWindow(obs_source_t *source)
{
	if (filters)
		filters->close();

	filters = new OBSBasicFilters(this, source);
	filters->Init();
	filters->setAttribute(Qt::WA_DeleteOnClose, true);
}

/* Qt callbacks for invokeMethod */

void OBSBasic::AddScene(OBSSource source)
{
	const char *name = obs_source_get_name(source);
	obs_scene_t *scene = obs_scene_from_source(source);

	QListWidgetItem *item = new QListWidgetItem(QT_UTF8(name));
	SetOBSRef(item, OBSScene(scene));
	ui->scenes->addItem(item);

	obs_hotkey_register_source(
		source, "OBSBasic.SelectScene",
		Str("Basic.Hotkeys.SelectScene"),
		[](void *data, obs_hotkey_id, obs_hotkey_t *, bool pressed) {
			OBSBasic *main = reinterpret_cast<OBSBasic *>(
				App()->GetMainWindow());

			auto potential_source =
				static_cast<obs_source_t *>(data);
			auto source = obs_source_get_ref(potential_source);
			if (source && pressed)
				main->SetCurrentScene(source);
			obs_source_release(source);
		},
		static_cast<obs_source_t *>(source));

	signal_handler_t *handler = obs_source_get_signal_handler(source);

	SignalContainer<OBSScene> container;
	container.ref = scene;
	container.handlers.assign({
		std::make_shared<OBSSignal>(handler, "item_add",
					    OBSBasic::SceneItemAdded, this),
		std::make_shared<OBSSignal>(handler, "item_select",
					    OBSBasic::SceneItemSelected, this),
		std::make_shared<OBSSignal>(handler, "item_deselect",
					    OBSBasic::SceneItemDeselected,
					    this),
		std::make_shared<OBSSignal>(handler, "reorder",
					    OBSBasic::SceneReordered, this),
		std::make_shared<OBSSignal>(handler, "refresh",
					    OBSBasic::SceneRefreshed, this),
	});

	item->setData(static_cast<int>(QtDataRole::OBSSignals), // storm
		      QVariant::fromValue(container));

	/* if the scene already has items (a duplicated scene) add them */
	auto addSceneItem = [this](obs_sceneitem_t *item) {
		AddSceneItem(item);
	};

	using addSceneItem_t = decltype(addSceneItem);

	obs_scene_enum_items(
		scene,
		[](obs_scene_t *, obs_sceneitem_t *item, void *param) {
			addSceneItem_t *func;
			func = reinterpret_cast<addSceneItem_t *>(param);
			(*func)(item);
			return true;
		},
		&addSceneItem);

	SaveProject();

	if (!disableSaving) {
		obs_source_t *source = obs_scene_get_source(scene);
		blog(LOG_INFO, "User added scene '%s'",
		     obs_source_get_name(source));

		OBSProjector::UpdateMultiviewProjectors();
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_SCENE_LIST_CHANGED);
}

void OBSBasic::RemoveScene(OBSSource source)
{
	obs_scene_t *scene = obs_scene_from_source(source);

	QListWidgetItem *sel = nullptr;
	int count = ui->scenes->count();

	for (int i = 0; i < count; i++) {
		auto item = ui->scenes->item(i);
		auto cur_scene = GetOBSRef<OBSScene>(item);
		if (cur_scene != scene)
			continue;

		sel = item;
		break;
	}

	if (sel != nullptr) {
		if (sel == ui->scenes->currentItem())
			ui->sources->Clear();
		delete sel;
	}

	SaveProject();

	if (!disableSaving) {
		blog(LOG_INFO, "User Removed scene '%s'",
		     obs_source_get_name(source));

		OBSProjector::UpdateMultiviewProjectors();
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_SCENE_LIST_CHANGED);
}

static bool select_one(obs_scene_t *scene, obs_sceneitem_t *item, void *param)
{
	obs_sceneitem_t *selectedItem =
		reinterpret_cast<obs_sceneitem_t *>(param);
	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, select_one, param);

	obs_sceneitem_select(item, (selectedItem == item));

	UNUSED_PARAMETER(scene);
	return true;
}

void OBSBasic::AddSceneItem(OBSSceneItem item)
{
	obs_scene_t *scene = obs_sceneitem_get_scene(item);

	if (GetCurrentScene() == scene)
		ui->sources->Add(item);

	SaveProject();

	if (!disableSaving) {
		obs_source_t *sceneSource = obs_scene_get_source(scene);
		obs_source_t *itemSource = obs_sceneitem_get_source(item);
		blog(LOG_INFO, "User added source '%s' (%s) to scene '%s'",
		     obs_source_get_name(itemSource),
		     obs_source_get_id(itemSource),
		     obs_source_get_name(sceneSource));

		obs_scene_enum_items(scene, select_one,
				     (obs_sceneitem_t *)item);
	}
}

void OBSBasic::UpdateSceneSelection(OBSSource source)
{
	if (source) {
		obs_scene_t *scene = obs_scene_from_source(source);
		const char *name = obs_source_get_name(source);

		if (!scene)
			return;

		QList<QListWidgetItem *> items =
			ui->scenes->findItems(QT_UTF8(name), Qt::MatchExactly);

		if (items.count()) {
			sceneChanging = true;
			ui->scenes->setCurrentItem(items.first());
			sceneChanging = false;

			OBSScene curScene =
				GetOBSRef<OBSScene>(ui->scenes->currentItem());
			if (api && scene != curScene)
				api->on_event(
					OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED);
		}
	}
}

static void RenameListValues(QListWidget *listWidget, const QString &newName,
			     const QString &prevName)
{
	QList<QListWidgetItem *> items =
		listWidget->findItems(prevName, Qt::MatchExactly);

	for (int i = 0; i < items.count(); i++)
		items[i]->setText(newName);
}

void OBSBasic::RenameSources(OBSSource source, QString newName,
			     QString prevName)
{
	RenameListValues(ui->scenes, newName, prevName);

	for (size_t i = 0; i < volumes.size(); i++) {
		if (volumes[i]->GetName().compare(prevName) == 0)
			volumes[i]->SetName(newName);
	}

	for (size_t i = 0; i < projectors.size(); i++) {
		if (projectors[i]->GetSource() == source)
			projectors[i]->RenameProjector(prevName, newName);
	}

	SaveProject();

	obs_scene_t *scene = obs_scene_from_source(source);
	if (scene)
		OBSProjector::UpdateMultiviewProjectors();
}

void OBSBasic::SelectSceneItem(OBSScene scene, OBSSceneItem item, bool select)
{
	SignalBlocker sourcesSignalBlocker(ui->sources);

	if (scene != GetCurrentScene() || ignoreSelectionUpdate)
		return;

	ui->sources->SelectItem(item, select);
}

static inline bool SourceMixerHidden(obs_source_t *source)
{
	obs_data_t *priv_settings = obs_source_get_private_settings(source);
	bool hidden = obs_data_get_bool(priv_settings, "mixer_hidden");
	obs_data_release(priv_settings);

	return hidden;
}

static inline void SetSourceMixerHidden(obs_source_t *source, bool hidden)
{
	obs_data_t *priv_settings = obs_source_get_private_settings(source);
	obs_data_set_bool(priv_settings, "mixer_hidden", hidden);
	obs_data_release(priv_settings);
}

void OBSBasic::GetAudioSourceFilters()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *source = vol->GetSource();

	CreateFiltersWindow(source);
}

void OBSBasic::GetAudioSourceProperties()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *source = vol->GetSource();

	CreatePropertiesWindow(source);
}

void OBSBasic::HideAudioControl()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *source = vol->GetSource();

	if (!SourceMixerHidden(source)) {
		SetSourceMixerHidden(source, true);
		DeactivateAudioSource(source);
	}
}

void OBSBasic::UnhideAllAudioControls()
{
	auto UnhideAudioMixer = [this](obs_source_t *source) /* -- */
	{
		if (!obs_source_active(source))
			return true;
		if (!SourceMixerHidden(source))
			return true;

		SetSourceMixerHidden(source, false);
		ActivateAudioSource(source);
		return true;
	};

	using UnhideAudioMixer_t = decltype(UnhideAudioMixer);

	auto PreEnum = [](void *data, obs_source_t *source) -> bool /* -- */
	{ return (*reinterpret_cast<UnhideAudioMixer_t *>(data))(source); };

	obs_enum_sources(PreEnum, &UnhideAudioMixer);
}

void OBSBasic::ToggleHideMixer()
{
	OBSSceneItem item = GetCurrentSceneItem();
	OBSSource source = obs_sceneitem_get_source(item);

	if (!SourceMixerHidden(source)) {
		SetSourceMixerHidden(source, true);
		DeactivateAudioSource(source);
	} else {
		SetSourceMixerHidden(source, false);
		ActivateAudioSource(source);
	}
}

void OBSBasic::MixerRenameSource()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	OBSSource source = vol->GetSource();

	const char *prevName = obs_source_get_name(source);

	for (;;) {
		string name;
		bool accepted = NameDialog::AskForName(
			this, QTStr("Basic.Main.MixerRename.Title"),
			QTStr("Basic.Main.MixerRename.Text"), name,
			QT_UTF8(prevName));
		if (!accepted)
			return;

		if (name.empty()) {
			OBSMessageBox::warning(this,
					       QTStr("NoNameEntered.Title"),
					       QTStr("NoNameEntered.Text"));
			continue;
		}

		OBSSource sourceTest = obs_get_source_by_name(name.c_str());
		obs_source_release(sourceTest);

		if (sourceTest) {
			OBSMessageBox::warning(this, QTStr("NameExists.Title"),
					       QTStr("NameExists.Text"));
			continue;
		}

		obs_source_set_name(source, name.c_str());
		break;
	}
}

static inline bool SourceVolumeLocked(obs_source_t *source)
{
	obs_data_t *priv_settings = obs_source_get_private_settings(source);
	bool lock = obs_data_get_bool(priv_settings, "volume_locked");
	obs_data_release(priv_settings);

	return lock;
}

void OBSBasic::LockVolumeControl(bool lock)
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *source = vol->GetSource();

	obs_data_t *priv_settings = obs_source_get_private_settings(source);
	obs_data_set_bool(priv_settings, "volume_locked", lock);
	obs_data_release(priv_settings);

	vol->EnableSlider(!lock);
}

void OBSBasic::VolControlContextMenu()
{
	VolControl *vol = reinterpret_cast<VolControl *>(sender());

	/* ------------------- */

	QAction lockAction(QTStr("LockVolume"), this);
	lockAction.setCheckable(true);
	lockAction.setChecked(SourceVolumeLocked(vol->GetSource()));

	QAction hideAction(QTStr("Hide"), this);
	QAction unhideAllAction(QTStr("UnhideAll"), this);
	QAction mixerRenameAction(QTStr("Rename"), this);

	QAction copyFiltersAction(QTStr("Copy.Filters"), this);
	QAction pasteFiltersAction(QTStr("Paste.Filters"), this);

	QAction filtersAction(QTStr("Filters"), this);
	QAction propertiesAction(QTStr("Properties"), this);
	QAction advPropAction(QTStr("Basic.MainMenu.Edit.AdvAudio"), this);

	QAction toggleControlLayoutAction(QTStr("VerticalLayout"), this);
	toggleControlLayoutAction.setCheckable(true);
	toggleControlLayoutAction.setChecked(config_get_bool(
		GetGlobalConfig(), "BasicWindow", "VerticalVolControl"));

	/* ------------------- */

	connect(&hideAction, &QAction::triggered, this,
		&OBSBasic::HideAudioControl, Qt::DirectConnection);
	connect(&unhideAllAction, &QAction::triggered, this,
		&OBSBasic::UnhideAllAudioControls, Qt::DirectConnection);
	connect(&lockAction, &QAction::toggled, this,
		&OBSBasic::LockVolumeControl, Qt::DirectConnection);
	connect(&mixerRenameAction, &QAction::triggered, this,
		&OBSBasic::MixerRenameSource, Qt::DirectConnection);

	connect(&copyFiltersAction, &QAction::triggered, this,
		&OBSBasic::AudioMixerCopyFilters, Qt::DirectConnection);
	connect(&pasteFiltersAction, &QAction::triggered, this,
		&OBSBasic::AudioMixerPasteFilters, Qt::DirectConnection);

	connect(&filtersAction, &QAction::triggered, this,
		&OBSBasic::GetAudioSourceFilters, Qt::DirectConnection);
	connect(&propertiesAction, &QAction::triggered, this,
		&OBSBasic::GetAudioSourceProperties, Qt::DirectConnection);
	connect(&advPropAction, &QAction::triggered, this,
		&OBSBasic::on_actionAdvAudioProperties_triggered,
		Qt::DirectConnection);

	/* ------------------- */

	connect(&toggleControlLayoutAction, &QAction::changed, this,
		&OBSBasic::ToggleVolControlLayout, Qt::DirectConnection);

	/* ------------------- */

	hideAction.setProperty("volControl",
			       QVariant::fromValue<VolControl *>(vol));
	lockAction.setProperty("volControl",
			       QVariant::fromValue<VolControl *>(vol));
	mixerRenameAction.setProperty("volControl",
				      QVariant::fromValue<VolControl *>(vol));

	copyFiltersAction.setProperty("volControl",
				      QVariant::fromValue<VolControl *>(vol));
	pasteFiltersAction.setProperty("volControl",
				       QVariant::fromValue<VolControl *>(vol));

	filtersAction.setProperty("volControl",
				  QVariant::fromValue<VolControl *>(vol));
	propertiesAction.setProperty("volControl",
				     QVariant::fromValue<VolControl *>(vol));

	/* ------------------- */

	if (copyFiltersString == nullptr)
		pasteFiltersAction.setEnabled(false);
	else
		pasteFiltersAction.setEnabled(true);

	QMenu popup;
	popup.addAction(&lockAction);
	popup.addSeparator();
	popup.addAction(&unhideAllAction);
	popup.addAction(&hideAction);
	popup.addAction(&mixerRenameAction);
	popup.addSeparator();
	popup.addAction(&copyFiltersAction);
	popup.addAction(&pasteFiltersAction);
	popup.addSeparator();
	popup.addAction(&toggleControlLayoutAction);
	popup.addSeparator();
	popup.addAction(&filtersAction);
	popup.addAction(&propertiesAction);
	popup.addAction(&advPropAction);
	popup.exec(QCursor::pos());
}

void OBSBasic::on_hMixerScrollArea_customContextMenuRequested()
{
	StackedMixerAreaContextMenuRequested();
}

void OBSBasic::on_vMixerScrollArea_customContextMenuRequested()
{
	StackedMixerAreaContextMenuRequested();
}

void OBSBasic::StackedMixerAreaContextMenuRequested()
{
	QAction unhideAllAction(QTStr("UnhideAll"), this);

	QAction advPropAction(QTStr("Basic.MainMenu.Edit.AdvAudio"), this);

	QAction toggleControlLayoutAction(QTStr("VerticalLayout"), this);
	toggleControlLayoutAction.setCheckable(true);
	toggleControlLayoutAction.setChecked(config_get_bool(
		GetGlobalConfig(), "BasicWindow", "VerticalVolControl"));

	/* ------------------- */

	connect(&unhideAllAction, &QAction::triggered, this,
		&OBSBasic::UnhideAllAudioControls, Qt::DirectConnection);

	connect(&advPropAction, &QAction::triggered, this,
		&OBSBasic::on_actionAdvAudioProperties_triggered,
		Qt::DirectConnection);

	/* ------------------- */

	connect(&toggleControlLayoutAction, &QAction::changed, this,
		&OBSBasic::ToggleVolControlLayout, Qt::DirectConnection);

	/* ------------------- */

	QMenu popup;
	popup.addAction(&unhideAllAction);
	popup.addSeparator();
	popup.addAction(&toggleControlLayoutAction);
	popup.addSeparator();
	popup.addAction(&advPropAction);
	popup.exec(QCursor::pos());
}

void OBSBasic::ToggleMixerLayout(bool vertical)
{
#if 0
	if (vertical) {
		ui->stackedMixerArea->setMinimumSize(180, 220);
		ui->stackedMixerArea->setCurrentIndex(1);
	} else {
		ui->stackedMixerArea->setMinimumSize(220, 0);
		ui->stackedMixerArea->setCurrentIndex(0);
	}
#endif
}

void OBSBasic::ToggleVolControlLayout()
{
	bool vertical = !config_get_bool(GetGlobalConfig(), "BasicWindow",
					 "VerticalVolControl");
	config_set_bool(GetGlobalConfig(), "BasicWindow", "VerticalVolControl",
			vertical);
	ToggleMixerLayout(vertical);

	// We need to store it so we can delete current and then add
	// at the right order
	vector<OBSSource> sources;
	for (size_t i = 0; i != volumes.size(); i++)
		sources.emplace_back(volumes[i]->GetSource());

	ClearVolumeControls();

	for (const auto &source : sources)
		ActivateAudioSource(source);
}

void OBSBasic::ActivateAudioSource(OBSSource source)
{
	if (SourceMixerHidden(source))
		return;
	if (!obs_source_audio_active(source))
		return;

	bool vertical = config_get_bool(GetGlobalConfig(), "BasicWindow",
					"VerticalVolControl");
	VolControl *vol = new VolControl(source, false, vertical);

	vol->EnableSlider(!SourceVolumeLocked(source));

	double meterDecayRate =
		config_get_double(basicConfig, "Audio", "MeterDecayRate");
	vol->SetMeterDecayRate(meterDecayRate);

	uint32_t peakMeterTypeIdx =
		config_get_uint(basicConfig, "Audio", "PeakMeterType");

	enum obs_peak_meter_type peakMeterType;
	switch (peakMeterTypeIdx) {
	case 0:
		peakMeterType = SAMPLE_PEAK_METER;
		break;
	case 1:
		peakMeterType = TRUE_PEAK_METER;
		break;
	default:
		peakMeterType = SAMPLE_PEAK_METER;
		break;
	}

	vol->setPeakMeterType(peakMeterType);

	vol->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(vol, &QWidget::customContextMenuRequested, this,
		&OBSBasic::VolControlContextMenu);
	connect(vol, &VolControl::ConfigClicked, this,
		&OBSBasic::VolControlContextMenu);

	InsertQObjectByName(volumes, vol);

	for (auto volume : volumes) {
        //ui->verticalLayout->addWidget(volume); //storm
#if 1
		if (vertical)
			ui->vVolControlLayout->addWidget(volume);
		else
			ui->hVolControlLayout->addWidget(volume);
#endif
	}
}

void OBSBasic::DeactivateAudioSource(OBSSource source)
{
	for (size_t i = 0; i < volumes.size(); i++) {
		if (volumes[i]->GetSource() == source) {
			delete volumes[i];
			volumes.erase(volumes.begin() + i);
			break;
		}
	}
}

bool OBSBasic::QueryRemoveSource(obs_source_t *source)
{
	if (obs_source_get_type(source) == OBS_SOURCE_TYPE_SCENE &&
	    !obs_source_is_group(source)) {
		int count = ui->scenes->count();

		if (count == 1) {
			OBSMessageBox::information(this,
						   QTStr("FinalScene.Title"),
						   QTStr("FinalScene.Text"));
			return false;
		}
	}

	const char *name = obs_source_get_name(source);

	QString text = QTStr("ConfirmRemove.Text");
	text.replace("$1", QT_UTF8(name));

	QMessageBox remove_source(this);
	remove_source.setText(text);
	QAbstractButton *Yes =
		remove_source.addButton(QTStr("Yes"), QMessageBox::YesRole);
	remove_source.addButton(QTStr("No"), QMessageBox::NoRole);
	remove_source.setIcon(QMessageBox::Question);
	remove_source.setWindowTitle(QTStr("ConfirmRemove.Title"));
	remove_source.exec();

	return Yes == remove_source.clickedButton();
}

#define UPDATE_CHECK_INTERVAL (60 * 60 * 24 * 4) /* 4 days */

#ifdef UPDATE_SPARKLE
void init_sparkle_updater(bool update_to_undeployed);
void trigger_sparkle_update();
#endif

void OBSBasic::TimedCheckForUpdates()
{
	if (!config_get_bool(App()->GlobalConfig(), "General",
			     "EnableAutoUpdates"))
		return;

#ifdef UPDATE_SPARKLE
	init_sparkle_updater(config_get_bool(App()->GlobalConfig(), "General",
					     "UpdateToUndeployed"));
#elif _WIN32
	long long lastUpdate = config_get_int(App()->GlobalConfig(), "General",
					      "LastUpdateCheck");
	uint32_t lastVersion =
		config_get_int(App()->GlobalConfig(), "General", "LastVersion");

	if (lastVersion < LIBOBS_API_VER) {
		lastUpdate = 0;
		config_set_int(App()->GlobalConfig(), "General",
			       "LastUpdateCheck", 0);
	}

	long long t = (long long)time(nullptr);
	long long secs = t - lastUpdate;

	if (secs > UPDATE_CHECK_INTERVAL)
		CheckForUpdates(false);
#endif
}

void OBSBasic::CheckForUpdates(bool manualUpdate)
{
#ifdef UPDATE_SPARKLE
	trigger_sparkle_update();
#elif _WIN32
	// ui->actionCheckForUpdates->setEnabled(false);

	if (updateCheckThread && updateCheckThread->isRunning())
		return;

	updateCheckThread.reset(new AutoUpdateThread(manualUpdate));
	updateCheckThread->start();
#endif

	UNUSED_PARAMETER(manualUpdate);
}

void OBSBasic::updateCheckFinished()
{
	// ui->actionCheckForUpdates->setEnabled(true);
}

void OBSBasic::DuplicateSelectedScene()
{
	OBSScene curScene = GetCurrentScene();

	if (!curScene)
		return;

	OBSSource curSceneSource = obs_scene_get_source(curScene);
	QString format{obs_source_get_name(curSceneSource)};
	format += " %1";

	int i = 2;
	QString placeHolderText = format.arg(i);
	obs_source_t *source = nullptr;
	while ((source = obs_get_source_by_name(QT_TO_UTF8(placeHolderText)))) {
		obs_source_release(source);
		placeHolderText = format.arg(++i);
	}

	for (;;) {
		string name;
		bool accepted = NameDialog::AskForName(
			this, QTStr("Basic.Main.AddSceneDlg.Title"),
			QTStr("Basic.Main.AddSceneDlg.Text"), name,
			placeHolderText);
		if (!accepted)
			return;

		if (name.empty()) {
			OBSMessageBox::warning(this,
					       QTStr("NoNameEntered.Title"),
					       QTStr("NoNameEntered.Text"));
			continue;
		}

		obs_source_t *source = obs_get_source_by_name(name.c_str());
		if (source) {
			OBSMessageBox::warning(this, QTStr("NameExists.Title"),
					       QTStr("NameExists.Text"));

			obs_source_release(source);
			continue;
		}

		obs_scene_t *scene = obs_scene_duplicate(curScene, name.c_str(),
							 OBS_SCENE_DUP_REFS);
		source = obs_scene_get_source(scene);
		SetCurrentScene(source, true);
		obs_scene_release(scene);

		break;
	}
}

void OBSBasic::RemoveSelectedScene()
{
	OBSScene scene = GetCurrentScene();
	if (scene) {
		obs_source_t *source = obs_scene_get_source(scene);
		if (QueryRemoveSource(source)) {
			obs_source_remove(source);

			if (api)
				api->on_event(
					OBS_FRONTEND_EVENT_SCENE_LIST_CHANGED);
		}
	}
}

void OBSBasic::RemoveSelectedSceneItem()
{
	OBSSceneItem item = GetCurrentSceneItem();
	if (item) {
		obs_source_t *source = obs_sceneitem_get_source(item);
		if (QueryRemoveSource(source))
			obs_sceneitem_remove(item);
	}
}

void OBSBasic::ReorderSources(OBSScene scene)
{
	if (scene != GetCurrentScene() || ui->sources->IgnoreReorder())
		return;

	ui->sources->ReorderItems();
	SaveProject();
}

void OBSBasic::RefreshSources(OBSScene scene)
{
	if (scene != GetCurrentScene() || ui->sources->IgnoreReorder())
		return;

	ui->sources->RefreshItems();
	SaveProject();
}

/* OBS Callbacks */

void OBSBasic::SceneReordered(void *data, calldata_t *params)
{
	OBSBasic *window = static_cast<OBSBasic *>(data);

	obs_scene_t *scene = (obs_scene_t *)calldata_ptr(params, "scene");

	QMetaObject::invokeMethod(window, "ReorderSources",
				  Q_ARG(OBSScene, OBSScene(scene)));
}

void OBSBasic::SceneRefreshed(void *data, calldata_t *params)
{
	OBSBasic *window = static_cast<OBSBasic *>(data);

	obs_scene_t *scene = (obs_scene_t *)calldata_ptr(params, "scene");

	QMetaObject::invokeMethod(window, "RefreshSources",
				  Q_ARG(OBSScene, OBSScene(scene)));
}

void OBSBasic::SceneItemAdded(void *data, calldata_t *params)
{
	OBSBasic *window = static_cast<OBSBasic *>(data);

	obs_sceneitem_t *item = (obs_sceneitem_t *)calldata_ptr(params, "item");

	QMetaObject::invokeMethod(window, "AddSceneItem",
				  Q_ARG(OBSSceneItem, OBSSceneItem(item)));
}

void OBSBasic::SceneItemSelected(void *data, calldata_t *params)
{
	OBSBasic *window = static_cast<OBSBasic *>(data);

	obs_scene_t *scene = (obs_scene_t *)calldata_ptr(params, "scene");
	obs_sceneitem_t *item = (obs_sceneitem_t *)calldata_ptr(params, "item");

	QMetaObject::invokeMethod(window, "SelectSceneItem",
				  Q_ARG(OBSScene, scene),
				  Q_ARG(OBSSceneItem, item), Q_ARG(bool, true));
}

void OBSBasic::SceneItemDeselected(void *data, calldata_t *params)
{
	OBSBasic *window = static_cast<OBSBasic *>(data);

	obs_scene_t *scene = (obs_scene_t *)calldata_ptr(params, "scene");
	obs_sceneitem_t *item = (obs_sceneitem_t *)calldata_ptr(params, "item");

	QMetaObject::invokeMethod(window, "SelectSceneItem",
				  Q_ARG(OBSScene, scene),
				  Q_ARG(OBSSceneItem, item),
				  Q_ARG(bool, false));
}

void OBSBasic::SourceCreated(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");

	if (obs_scene_from_source(source) != NULL)
		QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
					  "AddScene", WaitConnection(),
					  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceRemoved(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");

	if (obs_scene_from_source(source) != NULL)
		QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
					  "RemoveScene",
					  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceActivated(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");
	uint32_t flags = obs_source_get_output_flags(source);

	if (flags & OBS_SOURCE_AUDIO)
		QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
					  "ActivateAudioSource",
					  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceDeactivated(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");
	uint32_t flags = obs_source_get_output_flags(source);

	if (flags & OBS_SOURCE_AUDIO)
		QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
					  "DeactivateAudioSource",
					  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceAudioActivated(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");

	if (obs_source_active(source))
		QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
					  "ActivateAudioSource",
					  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceAudioDeactivated(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");
	QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
				  "DeactivateAudioSource",
				  Q_ARG(OBSSource, OBSSource(source)));
}

void OBSBasic::SourceRenamed(void *data, calldata_t *params)
{
	obs_source_t *source = (obs_source_t *)calldata_ptr(params, "source");
	const char *newName = calldata_string(params, "new_name");
	const char *prevName = calldata_string(params, "prev_name");

	QMetaObject::invokeMethod(static_cast<OBSBasic *>(data),
				  "RenameSources", Q_ARG(OBSSource, source),
				  Q_ARG(QString, QT_UTF8(newName)),
				  Q_ARG(QString, QT_UTF8(prevName)));

	blog(LOG_INFO, "Source '%s' renamed to '%s'", prevName, newName);
}

void OBSBasic::DrawBackdrop(float cx, float cy)
{
	if (!box)
		return;

	GS_DEBUG_MARKER_BEGIN(GS_DEBUG_COLOR_DEFAULT, "DrawBackdrop");

	gs_effect_t *solid = obs_get_base_effect(OBS_EFFECT_SOLID);
	gs_eparam_t *color = gs_effect_get_param_by_name(solid, "color");
	gs_technique_t *tech = gs_effect_get_technique(solid, "Solid");

	vec4 colorVal;
	vec4_set(&colorVal, 0.0f, 0.0f, 0.0f, 1.0f);
	gs_effect_set_vec4(color, &colorVal);

	gs_technique_begin(tech);
	gs_technique_begin_pass(tech, 0);
	gs_matrix_push();
	gs_matrix_identity();
	gs_matrix_scale3f(float(cx), float(cy), 1.0f);

	gs_load_vertexbuffer(box);
	gs_draw(GS_TRISTRIP, 0, 0);

	gs_matrix_pop();
	gs_technique_end_pass(tech);
	gs_technique_end(tech);

	gs_load_vertexbuffer(nullptr);

	GS_DEBUG_MARKER_END();
}

void OBSBasic::RenderMain(void *data, uint32_t cx, uint32_t cy)
{
	GS_DEBUG_MARKER_BEGIN(GS_DEBUG_COLOR_DEFAULT, "RenderMain");

	OBSBasic *window = static_cast<OBSBasic *>(data);
	obs_video_info ovi;

	obs_get_video_info(&ovi);

	window->previewCX = int(window->previewScale * float(ovi.base_width));
	window->previewCY = int(window->previewScale * float(ovi.base_height));

	gs_viewport_push();
	gs_projection_push();

	obs_display_t *display = window->ui->preview->GetDisplay();
	uint32_t width, height;
	obs_display_size(display, &width, &height);
	float right = float(width) - window->previewX;
	float bottom = float(height) - window->previewY;

	gs_ortho(-window->previewX, right, -window->previewY, bottom, -100.0f,
		 100.0f);

	window->ui->preview->DrawOverflow();

	/* --------------------------------------- */

	gs_ortho(0.0f, float(ovi.base_width), 0.0f, float(ovi.base_height),
		 -100.0f, 100.0f);
	gs_set_viewport(window->previewX, window->previewY, window->previewCX,
			window->previewCY);

	if (window->IsPreviewProgramMode()) {
		window->DrawBackdrop(float(ovi.base_width),
				     float(ovi.base_height));

		OBSScene scene = window->GetCurrentScene();
		obs_source_t *source = obs_scene_get_source(scene);
		if (source)
			obs_source_video_render(source);
	} else {
		obs_render_main_texture_src_color_only();
	}
	gs_load_vertexbuffer(nullptr);

	/* --------------------------------------- */

	gs_ortho(-window->previewX, right, -window->previewY, bottom, -100.0f,
		 100.0f);
	gs_reset_viewport();

	window->ui->preview->DrawSceneEditing();

	/* --------------------------------------- */

	gs_projection_pop();
	gs_viewport_pop();

	GS_DEBUG_MARKER_END();

	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}

/* Main class functions */

obs_service_t *OBSBasic::GetService()
{
	if (!service) {
		service =
			obs_service_create("rtmp_common", NULL, NULL, nullptr);
		obs_service_release(service);
	}
	return service;
}

void OBSBasic::SetService(obs_service_t *newService)
{
	if (newService)
		service = newService;
}

int OBSBasic::GetTransitionDuration()
{
	return iTransitionDuration;// ui->transitionDuration->value();
}
#if 0
bool OBSBasic::StreamingActive() const
{
	if (!outputHandler)
		return false;
	return outputHandler->StreamingActive();
}
#endif
bool OBSBasic::Active() const
{
	if (!outputHandler)
		return false;
	return outputHandler->Active();
}

#ifdef _WIN32
#define IS_WIN32 1
#else
#define IS_WIN32 0
#endif

static inline int AttemptToResetVideo(struct obs_video_info *ovi)
{
	return obs_reset_video(ovi);
}

static inline enum obs_scale_type GetScaleType(ConfigFile &basicConfig)
{
	const char *scaleTypeStr =
		config_get_string(basicConfig, "Video", "ScaleType");

	if (astrcmpi(scaleTypeStr, "bilinear") == 0)
		return OBS_SCALE_BILINEAR;
	else if (astrcmpi(scaleTypeStr, "lanczos") == 0)
		return OBS_SCALE_LANCZOS;
	else if (astrcmpi(scaleTypeStr, "area") == 0)
		return OBS_SCALE_AREA;
	else
		return OBS_SCALE_BICUBIC;
}

static inline enum video_format GetVideoFormatFromName(const char *name)
{
	if (astrcmpi(name, "I420") == 0)
		return VIDEO_FORMAT_I420;
	else if (astrcmpi(name, "NV12") == 0)
		return VIDEO_FORMAT_NV12;
	else if (astrcmpi(name, "I444") == 0)
		return VIDEO_FORMAT_I444;
#if 0 //currently unsupported
	else if (astrcmpi(name, "YVYU") == 0)
		return VIDEO_FORMAT_YVYU;
	else if (astrcmpi(name, "YUY2") == 0)
		return VIDEO_FORMAT_YUY2;
	else if (astrcmpi(name, "UYVY") == 0)
		return VIDEO_FORMAT_UYVY;
#endif
	else
		return VIDEO_FORMAT_RGBA;
}

void OBSBasic::ResetUI()
{
	bool studioPortraitLayout = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "StudioPortraitLayout");

	bool labels = config_get_bool(GetGlobalConfig(), "BasicWindow",
				      "StudioModeLabels");

	if (studioPortraitLayout)
		ui->previewLayout->setDirection(QBoxLayout::TopToBottom);
	else
		ui->previewLayout->setDirection(QBoxLayout::LeftToRight);
#if 0
	if (previewProgramMode)
		ui->previewLabel->setHidden(!labels);
#endif
	if (programLabel)
		programLabel->setHidden(!labels);
}

int OBSBasic::ResetVideo()
{
	if (outputHandler && outputHandler->Active())
		return OBS_VIDEO_CURRENTLY_ACTIVE;

	ProfileScope("OBSBasic::ResetVideo");

	struct obs_video_info ovi;
	int ret;

	GetConfigFPS(ovi.fps_num, ovi.fps_den);

	const char *colorFormat =
		config_get_string(basicConfig, "Video", "ColorFormat");
	const char *colorSpace =
		config_get_string(basicConfig, "Video", "ColorSpace");
	const char *colorRange =
		config_get_string(basicConfig, "Video", "ColorRange");

	ovi.graphics_module = App()->GetRenderModule();
	ovi.base_width =
		(uint32_t)config_get_uint(basicConfig, "Video", "BaseCX");
	ovi.base_height =
		(uint32_t)config_get_uint(basicConfig, "Video", "BaseCY");
	ovi.output_width =
		(uint32_t)config_get_uint(basicConfig, "Video", "OutputCX");
	ovi.output_height =
		(uint32_t)config_get_uint(basicConfig, "Video", "OutputCY");
	ovi.output_format = GetVideoFormatFromName(colorFormat);
	ovi.colorspace = astrcmpi(colorSpace, "601") == 0 ? VIDEO_CS_601
							  : VIDEO_CS_709;
	ovi.range = astrcmpi(colorRange, "Full") == 0 ? VIDEO_RANGE_FULL
						      : VIDEO_RANGE_PARTIAL;
	ovi.adapter =
		config_get_uint(App()->GlobalConfig(), "Video", "AdapterIdx");
	ovi.gpu_conversion = true;
	ovi.scale_type = GetScaleType(basicConfig);

	if (ovi.base_width == 0 || ovi.base_height == 0) {
		ovi.base_width = 1920;
		ovi.base_height = 1080;
		config_set_uint(basicConfig, "Video", "BaseCX", 1920);
		config_set_uint(basicConfig, "Video", "BaseCY", 1080);
	}

	if (ovi.output_width == 0 || ovi.output_height == 0) {
		ovi.output_width = ovi.base_width;
		ovi.output_height = ovi.base_height;
		config_set_uint(basicConfig, "Video", "OutputCX",
				ovi.base_width);
		config_set_uint(basicConfig, "Video", "OutputCY",
				ovi.base_height);
	}

	ret = AttemptToResetVideo(&ovi);
	if (IS_WIN32 && ret != OBS_VIDEO_SUCCESS) {
		if (ret == OBS_VIDEO_CURRENTLY_ACTIVE) {
			blog(LOG_WARNING, "Tried to reset when "
					  "already active");
			return ret;
		}

		/* Try OpenGL if DirectX fails on windows */
		if (astrcmpi(ovi.graphics_module, DL_OPENGL) != 0) {
			blog(LOG_WARNING,
			     "Failed to initialize obs video (%d) "
			     "with graphics_module='%s', retrying "
			     "with graphics_module='%s'",
			     ret, ovi.graphics_module, DL_OPENGL);
			ovi.graphics_module = DL_OPENGL;
			ret = AttemptToResetVideo(&ovi);
		}
	} else if (ret == OBS_VIDEO_SUCCESS) {
		ResizePreview(ovi.base_width, ovi.base_height);
		if (program)
			ResizeProgram(ovi.base_width, ovi.base_height);
	}

	if (ret == OBS_VIDEO_SUCCESS) {
		OBSBasicStats::InitializeValues();
		OBSProjector::UpdateMultiviewProjectors();
	}

	return ret;
}

bool OBSBasic::ResetAudio()
{
	ProfileScope("OBSBasic::ResetAudio");

	struct obs_audio_info ai;
	ai.samples_per_sec =
		config_get_uint(basicConfig, "Audio", "SampleRate");

	const char *channelSetupStr =
		config_get_string(basicConfig, "Audio", "ChannelSetup");

	if (strcmp(channelSetupStr, "Mono") == 0)
		ai.speakers = SPEAKERS_MONO;
	else if (strcmp(channelSetupStr, "2.1") == 0)
		ai.speakers = SPEAKERS_2POINT1;
	else if (strcmp(channelSetupStr, "4.0") == 0)
		ai.speakers = SPEAKERS_4POINT0;
	else if (strcmp(channelSetupStr, "4.1") == 0)
		ai.speakers = SPEAKERS_4POINT1;
	else if (strcmp(channelSetupStr, "5.1") == 0)
		ai.speakers = SPEAKERS_5POINT1;
	else if (strcmp(channelSetupStr, "7.1") == 0)
		ai.speakers = SPEAKERS_7POINT1;
	else
		ai.speakers = SPEAKERS_STEREO;

	return obs_reset_audio(&ai);
}

void OBSBasic::ResetAudioDevice(const char *sourceId, const char *deviceId,
				const char *deviceDesc, int channel)
{
	bool disable = deviceId && strcmp(deviceId, "disabled") == 0;
	obs_source_t *source;
	obs_data_t *settings;

	source = obs_get_output_source(channel);
	if (source) {
		if (disable) {
			obs_set_output_source(channel, nullptr);
		} else {
			settings = obs_source_get_settings(source);
			const char *oldId =
				obs_data_get_string(settings, "device_id");
			if (strcmp(oldId, deviceId) != 0) {
				obs_data_set_string(settings, "device_id",
						    deviceId);
				obs_source_update(source, settings);
			}
			obs_data_release(settings);
		}

		obs_source_release(source);

	} else if (!disable) {
		settings = obs_data_create();
		obs_data_set_string(settings, "device_id", deviceId);
		source = obs_source_create(sourceId, deviceDesc, settings,
					   nullptr);
		obs_data_release(settings);

		obs_set_output_source(channel, source);
		obs_source_release(source);
	}
}

void OBSBasic::ResizePreview(uint32_t cx, uint32_t cy)
{
	QSize targetSize;
	bool isFixedScaling;
	obs_video_info ovi;

	/* resize preview panel to fix to the top section of the window */
	targetSize = GetPixelSize(ui->preview);

	isFixedScaling = ui->preview->IsFixedScaling();
	obs_get_video_info(&ovi);

	if (isFixedScaling) {
		previewScale = ui->preview->GetScalingAmount();
		GetCenterPosFromFixedScale(
			int(cx), int(cy),
			targetSize.width() - PREVIEW_EDGE_SIZE * 2,
			targetSize.height() - PREVIEW_EDGE_SIZE * 2, previewX,
			previewY, previewScale);
		previewX += ui->preview->GetScrollX();
		previewY += ui->preview->GetScrollY();

	} else {
		GetScaleAndCenterPos(int(cx), int(cy),
				     targetSize.width() - PREVIEW_EDGE_SIZE * 2,
				     targetSize.height() -
					     PREVIEW_EDGE_SIZE * 2,
				     previewX, previewY, previewScale);
	}

	previewX += float(PREVIEW_EDGE_SIZE);
	previewY += float(PREVIEW_EDGE_SIZE);
}

void OBSBasic::CloseDialogs()
{
	QList<QDialog *> childDialogs = this->findChildren<QDialog *>();
	if (!childDialogs.isEmpty()) {
		for (int i = 0; i < childDialogs.size(); ++i) {
			childDialogs.at(i)->close();
		}
	}

	if (!stats.isNull())
		stats->close(); //call close to save Stats geometry
	if (!remux.isNull())
		remux->close();
}

void OBSBasic::EnumDialogs()
{
	visDialogs.clear();
	modalDialogs.clear();
	visMsgBoxes.clear();

	/* fill list of Visible dialogs and Modal dialogs */
	QList<QDialog *> dialogs = findChildren<QDialog *>();
	for (QDialog *dialog : dialogs) {
		if (dialog->isVisible())
			visDialogs.append(dialog);
		if (dialog->isModal())
			modalDialogs.append(dialog);
	}

	/* fill list of Visible message boxes */
	QList<QMessageBox *> msgBoxes = findChildren<QMessageBox *>();
	for (QMessageBox *msgbox : msgBoxes) {
		if (msgbox->isVisible())
			visMsgBoxes.append(msgbox);
	}
}

void OBSBasic::ClearSceneData()
{
	disableSaving++;

	CloseDialogs();

	ClearVolumeControls();
	ClearListItems(ui->scenes);
	ui->sources->Clear();
	ClearQuickTransitions();
	ui->transitions->clear();

	for (size_t i = 0; i < projectors.size(); i++) {
		if (projectors[i])
			delete projectors[i];
	}

	projectors.clear();

	obs_set_output_source(0, nullptr);
	obs_set_output_source(1, nullptr);
	obs_set_output_source(2, nullptr);
	obs_set_output_source(3, nullptr);
	obs_set_output_source(4, nullptr);
	obs_set_output_source(5, nullptr);
	lastScene = nullptr;
	swapScene = nullptr;
	programScene = nullptr;

	auto cb = [](void *unused, obs_source_t *source) {
		obs_source_remove(source);
		UNUSED_PARAMETER(unused);
		return true;
	};

	obs_enum_sources(cb, nullptr);

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_SCENE_COLLECTION_CLEANUP);

	disableSaving--;

	blog(LOG_INFO, "All scene data cleared");
	blog(LOG_INFO, "------------------------------------------------");
}

void OBSBasic::closeEvent(QCloseEvent *event)
{
	/* Do not close window if inside of a temporary event loop because we
	 * could be inside of an Auth::LoadUI call.  Keep trying once per
	 * second until we've exit any known sub-loops. */
	if (os_atomic_load_long(&insideEventLoop) != 0) {
		QTimer::singleShot(1000, this, SLOT(close()));
		event->ignore();
		return;
	}

	if (isVisible())
		config_set_string(App()->GlobalConfig(), "BasicWindow",
				  "geometry",
				  saveGeometry().toBase64().constData());

	if (outputHandler && outputHandler->Active()) {
		SetShowing(true);

		QMessageBox::StandardButton button = OBSMessageBox::question(
			this, QTStr("ConfirmExit.Title"),
			QTStr("ConfirmExit.Text"));

		if (button == QMessageBox::No) {
			event->ignore();
			restart = false;
			return;
		}
	}

	QWidget::closeEvent(event);
	if (!event->isAccepted())
		return;

	blog(LOG_INFO, SHUTDOWN_SEPARATOR);

	closing = true;

	if (introCheckThread)
		introCheckThread->wait();
	if (whatsNewInitThread)
		whatsNewInitThread->wait();
	if (updateCheckThread)
		updateCheckThread->wait();
	if (logUploadThread)
		logUploadThread->wait();

	signalHandlers.clear();

	Auth::Save();
	SaveProjectNow();
	config_save_safe(basicConfig, "tmp", nullptr); //storm  window-basic-settings.cpp=> here
	auth.reset();

	delete extraBrowsers;

	//config_set_string(App()->GlobalConfig(), "BasicWindow", "DockState",
	//		  saveState().toBase64().constData());

#ifdef BROWSER_AVAILABLE
	SaveExtraBrowserDocks();
	ClearExtraBrowserDocks();
#endif

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_EXIT);

	disableSaving++;

	/* Clear all scene data (dialogs, widgets, widget sub-items, scenes,
	 * sources, etc) so that all references are released before shutdown */
	ClearSceneData();

	// App()->quit(); // storm
}

void OBSBasic::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange) {
		QWindowStateChangeEvent *stateEvent =
			(QWindowStateChangeEvent *)event;

		if (isMinimized()) {
			if (trayIcon && trayIcon->isVisible() &&
			    sysTrayMinimizeToTray()) {
				ToggleShowHide();
			}

			if (previewEnabled)
				EnablePreviewDisplay(false);
		} else if (stateEvent->oldState() & Qt::WindowMinimized &&
			   isVisible()) {
			if (previewEnabled)
				EnablePreviewDisplay(true);
		}
	}
}

void OBSBasic::on_actionShow_Recordings_triggered()
{
	const char *mode = config_get_string(basicConfig, "Output", "Mode");
	const char *type = config_get_string(basicConfig, "AdvOut", "RecType");
	const char *adv_path =
		strcmp(type, "Standard")
			? config_get_string(basicConfig, "AdvOut", "FFFilePath")
			: config_get_string(basicConfig, "AdvOut",
					    "RecFilePath");
	const char *path = strcmp(mode, "Advanced")
				   ? config_get_string(basicConfig,
						       "SimpleOutput",
						       "FilePath")
				   : adv_path;
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void OBSBasic::on_actionRemux_triggered()
{
	if (!remux.isNull()) {
		remux->show();
		remux->raise();
		return;
	}

	const char *mode = config_get_string(basicConfig, "Output", "Mode");
	const char *path = strcmp(mode, "Advanced")
				   ? config_get_string(basicConfig,
						       "SimpleOutput",
						       "FilePath")
				   : config_get_string(basicConfig, "AdvOut",
						       "RecFilePath");

	OBSRemux *remuxDlg;
	remuxDlg = new OBSRemux(path, this);
	remuxDlg->show();
	remux = remuxDlg;
}

void OBSBasic::on_action_Settings_triggered()
{
	static bool settings_already_executing = false;

	/* Do not load settings window if inside of a temporary event loop
	 * because we could be inside of an Auth::LoadUI call.  Keep trying
	 * once per second until we've exit any known sub-loops. */
	if (os_atomic_load_long(&insideEventLoop) != 0) {
		QTimer::singleShot(1000, this,
				   SLOT(on_action_Settings_triggered()));
		return;
	}

	if (settings_already_executing) {
		return;
	}

	settings_already_executing = true;

	{
		OBSBasicSettings settings(this);
		settings.exec();
	}

	SystemTray(false);

	settings_already_executing = false;

	if (restart) {
		QMessageBox::StandardButton button = OBSMessageBox::question(
			this, QTStr("Restart"), QTStr("NeedsRestart"));

		if (button == QMessageBox::Yes)
			close();
		else
			restart = false;
	}
}

void OBSBasic::on_actionAdvAudioProperties_triggered()
{
	if (advAudioWindow != nullptr) {
		advAudioWindow->raise();
		return;
	}

	bool iconsVisible = config_get_bool(App()->GlobalConfig(),
					    "BasicWindow", "ShowSourceIcons");

	advAudioWindow = new OBSBasicAdvAudio(this);
	advAudioWindow->show();
	advAudioWindow->setAttribute(Qt::WA_DeleteOnClose, true);
	advAudioWindow->SetIconsVisible(iconsVisible);

	connect(advAudioWindow, SIGNAL(destroyed()), this,
		SLOT(on_advAudioProps_destroyed()));
}

void OBSBasic::on_advAudioProps_clicked()
{
	on_actionAdvAudioProperties_triggered();
}

void OBSBasic::on_advAudioProps_destroyed()
{
	advAudioWindow = nullptr;
}

void OBSBasic::on_scenes_currentItemChanged(QListWidgetItem *current,
					    QListWidgetItem *prev)
{
	obs_source_t *source = NULL;

	if (sceneChanging)
		return;

	if (current) {
		obs_scene_t *scene;

		scene = GetOBSRef<OBSScene>(current);
		source = obs_scene_get_source(scene);
	}

	SetCurrentScene(source);

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED);

	UNUSED_PARAMETER(prev);
}

void OBSBasic::EditSceneName()
{
	QListWidgetItem *item = ui->scenes->currentItem();
	Qt::ItemFlags flags = item->flags();

	item->setFlags(flags | Qt::ItemIsEditable);
	ui->scenes->editItem(item);
	item->setFlags(flags);
}

void OBSBasic::AddProjectorMenuMonitors(QMenu *parent, QObject *target,
					const char *slot)
{
	QAction *action;
	QList<QScreen *> screens = QGuiApplication::screens();
	for (int i = 0; i < screens.size(); i++) {
		QScreen *screen = screens[i];
		QRect screenGeometry = screen->geometry();
		QString name = "";
#ifdef _WIN32
		QTextStream fullname(&name);
		fullname << GetMonitorName(screen->name());
		fullname << " (";
		fullname << (i + 1);
		fullname << ")";
#elif defined(__APPLE__)
		name = screen->name();
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
		name = screen->model().simplified();

		if (name.length() > 1 && name.endsWith("-"))
			name.chop(1);
#endif
		name = name.simplified();

		if (name.length() == 0) {
			name = QString("%1 %2")
				       .arg(QTStr("Display"))
				       .arg(QString::number(i + 1));
		}
		QString str =
			QString("%1: %2x%3 @ %4,%5")
				.arg(name,
				     QString::number(screenGeometry.width()),
				     QString::number(screenGeometry.height()),
				     QString::number(screenGeometry.x()),
				     QString::number(screenGeometry.y()));

		action = parent->addAction(str, target, slot);
		action->setProperty("monitor", i);
	}
}

void OBSBasic::on_scenes_customContextMenuRequested(const QPoint &pos)
{
	QListWidgetItem *item = ui->scenes->itemAt(pos);

	QMenu popup(this);
	QMenu order(QTStr("Basic.MainMenu.Edit.Order"), this);

	popup.addAction(QTStr("Add"), this,
			SLOT(on_actionAddScene_triggered()));

	if (item) {
		QAction *pasteFilters =
			new QAction(QTStr("Paste.Filters"), this);
		pasteFilters->setEnabled(copyFiltersString);
		connect(pasteFilters, SIGNAL(triggered()), this,
			SLOT(ScenePasteFilters()));

		popup.addSeparator();
		popup.addAction(QTStr("Duplicate"), this,
				SLOT(DuplicateSelectedScene()));
		popup.addAction(QTStr("Copy.Filters"), this,
				SLOT(SceneCopyFilters()));
		popup.addAction(pasteFilters);
		popup.addSeparator();
		popup.addAction(QTStr("Rename"), this, SLOT(EditSceneName()));
		popup.addAction(QTStr("Remove"), this,
				SLOT(RemoveSelectedScene()));
		popup.addSeparator();

		order.addAction(QTStr("Basic.MainMenu.Edit.Order.MoveUp"), this,
				SLOT(on_actionSceneUp_triggered()));
		order.addAction(QTStr("Basic.MainMenu.Edit.Order.MoveDown"),
				this, SLOT(on_actionSceneDown_triggered()));
		order.addSeparator();
		order.addAction(QTStr("Basic.MainMenu.Edit.Order.MoveToTop"),
				this, SLOT(MoveSceneToTop()));
		order.addAction(QTStr("Basic.MainMenu.Edit.Order.MoveToBottom"),
				this, SLOT(MoveSceneToBottom()));
		popup.addMenu(&order);

		popup.addSeparator();

		delete sceneProjectorMenu;
		sceneProjectorMenu = new QMenu(QTStr("SceneProjector"));
		AddProjectorMenuMonitors(sceneProjectorMenu, this,
					 SLOT(OpenSceneProjector()));
		popup.addMenu(sceneProjectorMenu);

		QAction *sceneWindow = popup.addAction(
			QTStr("SceneWindow"), this, SLOT(OpenSceneWindow()));

		popup.addAction(sceneWindow);
		popup.addSeparator();
		popup.addAction(QTStr("Filters"), this,
				SLOT(OpenSceneFilters()));

		popup.addSeparator();

		delete perSceneTransitionMenu;
		perSceneTransitionMenu = CreatePerSceneTransitionMenu();
		popup.addMenu(perSceneTransitionMenu);

		/* ---------------------- */

		QAction *multiviewAction =
			popup.addAction(QTStr("ShowInMultiview"));

		OBSSource source = GetCurrentSceneSource();
		OBSData data = obs_source_get_private_settings(source);
		obs_data_release(data);

		obs_data_set_default_bool(data, "show_in_multiview", true);
		bool show = obs_data_get_bool(data, "show_in_multiview");

		multiviewAction->setCheckable(true);
		multiviewAction->setChecked(show);

		auto showInMultiview = [](OBSData data) {
			bool show =
				obs_data_get_bool(data, "show_in_multiview");
			obs_data_set_bool(data, "show_in_multiview", !show);
			OBSProjector::UpdateMultiviewProjectors();
		};

		connect(multiviewAction, &QAction::triggered,
			std::bind(showInMultiview, data));
	}

	popup.addSeparator();

	bool grid = ui->scenes->GetGridMode();

	QAction *gridAction = new QAction(grid ? QTStr("Basic.Main.ListMode")
					       : QTStr("Basic.Main.GridMode"),
					  this);
	connect(gridAction, SIGNAL(triggered()), this,
		SLOT(on_actionGridMode_triggered()));
	popup.addAction(gridAction);

	popup.exec(QCursor::pos());
}

void OBSBasic::on_actionGridMode_triggered()
{
	bool gridMode = !ui->scenes->GetGridMode();
	ui->scenes->SetGridMode(gridMode);
}

void OBSBasic::on_actionAddScene_triggered()
{
	string name;
	QString format{QTStr("Basic.Main.DefaultSceneName.Text")};

	int i = 2;
	QString placeHolderText = format.arg(i);
	obs_source_t *source = nullptr;
	while ((source = obs_get_source_by_name(QT_TO_UTF8(placeHolderText)))) {
		obs_source_release(source);
		placeHolderText = format.arg(++i);
	}

	bool accepted = NameDialog::AskForName(
		this, QTStr("Basic.Main.AddSceneDlg.Title"),
		QTStr("Basic.Main.AddSceneDlg.Text"), name, placeHolderText);

	if (accepted) {
		if (name.empty()) {
			OBSMessageBox::warning(this,
					       QTStr("NoNameEntered.Title"),
					       QTStr("NoNameEntered.Text"));
			on_actionAddScene_triggered();
			return;
		}

		obs_source_t *source = obs_get_source_by_name(name.c_str());
		if (source) {
			OBSMessageBox::warning(this, QTStr("NameExists.Title"),
					       QTStr("NameExists.Text"));

			obs_source_release(source);
			on_actionAddScene_triggered();
			return;
		}

		obs_scene_t *scene = obs_scene_create(name.c_str());
		source = obs_scene_get_source(scene);
		SetCurrentScene(source);
		obs_scene_release(scene);
	}
}

void OBSBasic::on_actionRemoveScene_triggered()
{
	OBSScene scene = GetCurrentScene();
	obs_source_t *source = obs_scene_get_source(scene);

	if (source && QueryRemoveSource(source))
		obs_source_remove(source);
}

void OBSBasic::ChangeSceneIndex(bool relative, int offset, int invalidIdx)
{
	int idx = ui->scenes->currentRow();
	if (idx == -1 || idx == invalidIdx)
		return;

	sceneChanging = true;

	QListWidgetItem *item = ui->scenes->takeItem(idx);

	if (!relative)
		idx = 0;

	ui->scenes->insertItem(idx + offset, item);
	ui->scenes->setCurrentRow(idx + offset);
	item->setSelected(true);

	sceneChanging = false;

	OBSProjector::UpdateMultiviewProjectors();
}

void OBSBasic::on_actionSceneUp_triggered()
{
	ChangeSceneIndex(true, -1, 0);
}

void OBSBasic::on_actionSceneDown_triggered()
{
	ChangeSceneIndex(true, 1, ui->scenes->count() - 1);
}

void OBSBasic::MoveSceneToTop()
{
	ChangeSceneIndex(false, 0, 0);
}

void OBSBasic::MoveSceneToBottom()
{
	ChangeSceneIndex(false, ui->scenes->count() - 1,
			 ui->scenes->count() - 1);
}

void OBSBasic::EditSceneItemName()
{
	int idx = GetTopSelectedSourceItem();
	ui->sources->Edit(idx);
}

void OBSBasic::SetDeinterlacingMode()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	obs_deinterlace_mode mode =
		(obs_deinterlace_mode)action->property("mode").toInt();
	OBSSceneItem sceneItem = GetCurrentSceneItem();
	obs_source_t *source = obs_sceneitem_get_source(sceneItem);

	obs_source_set_deinterlace_mode(source, mode);
}

void OBSBasic::SetDeinterlacingOrder()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	obs_deinterlace_field_order order =
		(obs_deinterlace_field_order)action->property("order").toInt();
	OBSSceneItem sceneItem = GetCurrentSceneItem();
	obs_source_t *source = obs_sceneitem_get_source(sceneItem);

	obs_source_set_deinterlace_field_order(source, order);
}

QMenu *OBSBasic::AddDeinterlacingMenu(QMenu *menu, obs_source_t *source)
{
	obs_deinterlace_mode deinterlaceMode =
		obs_source_get_deinterlace_mode(source);
	obs_deinterlace_field_order deinterlaceOrder =
		obs_source_get_deinterlace_field_order(source);
	QAction *action;

#define ADD_MODE(name, mode)                                    \
	action = menu->addAction(QTStr("" name), this,          \
				 SLOT(SetDeinterlacingMode())); \
	action->setProperty("mode", (int)mode);                 \
	action->setCheckable(true);                             \
	action->setChecked(deinterlaceMode == mode);

	ADD_MODE("Disable", OBS_DEINTERLACE_MODE_DISABLE);
	ADD_MODE("Deinterlacing.Discard", OBS_DEINTERLACE_MODE_DISCARD);
	ADD_MODE("Deinterlacing.Retro", OBS_DEINTERLACE_MODE_RETRO);
	ADD_MODE("Deinterlacing.Blend", OBS_DEINTERLACE_MODE_BLEND);
	ADD_MODE("Deinterlacing.Blend2x", OBS_DEINTERLACE_MODE_BLEND_2X);
	ADD_MODE("Deinterlacing.Linear", OBS_DEINTERLACE_MODE_LINEAR);
	ADD_MODE("Deinterlacing.Linear2x", OBS_DEINTERLACE_MODE_LINEAR_2X);
	ADD_MODE("Deinterlacing.Yadif", OBS_DEINTERLACE_MODE_YADIF);
	ADD_MODE("Deinterlacing.Yadif2x", OBS_DEINTERLACE_MODE_YADIF_2X);
#undef ADD_MODE

	menu->addSeparator();

#define ADD_ORDER(name, order)                                       \
	action = menu->addAction(QTStr("Deinterlacing." name), this, \
				 SLOT(SetDeinterlacingOrder()));     \
	action->setProperty("order", (int)order);                    \
	action->setCheckable(true);                                  \
	action->setChecked(deinterlaceOrder == order);

	ADD_ORDER("TopFieldFirst", OBS_DEINTERLACE_FIELD_ORDER_TOP);
	ADD_ORDER("BottomFieldFirst", OBS_DEINTERLACE_FIELD_ORDER_BOTTOM);
#undef ADD_ORDER

	return menu;
}

void OBSBasic::SetScaleFilter()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	obs_scale_type mode = (obs_scale_type)action->property("mode").toInt();
	OBSSceneItem sceneItem = GetCurrentSceneItem();

	obs_sceneitem_set_scale_filter(sceneItem, mode);
}

QMenu *OBSBasic::AddScaleFilteringMenu(QMenu *menu, obs_sceneitem_t *item)
{
	obs_scale_type scaleFilter = obs_sceneitem_get_scale_filter(item);
	QAction *action;

#define ADD_MODE(name, mode)                                                   \
	action =                                                               \
		menu->addAction(QTStr("" name), this, SLOT(SetScaleFilter())); \
	action->setProperty("mode", (int)mode);                                \
	action->setCheckable(true);                                            \
	action->setChecked(scaleFilter == mode);

	ADD_MODE("Disable", OBS_SCALE_DISABLE);
	ADD_MODE("ScaleFiltering.Point", OBS_SCALE_POINT);
	ADD_MODE("ScaleFiltering.Bilinear", OBS_SCALE_BILINEAR);
	ADD_MODE("ScaleFiltering.Bicubic", OBS_SCALE_BICUBIC);
	ADD_MODE("ScaleFiltering.Lanczos", OBS_SCALE_LANCZOS);
	ADD_MODE("ScaleFiltering.Area", OBS_SCALE_AREA);
#undef ADD_MODE

	return menu;
}

QMenu *OBSBasic::AddBackgroundColorMenu(QMenu *menu,
					QWidgetAction *widgetAction,
					ColorSelect *select,
					obs_sceneitem_t *item)
{
	QAction *action;

	menu->setStyleSheet(QString(
		"*[bgColor=\"1\"]{background-color:rgba(255,68,68,33%);}"
		"*[bgColor=\"2\"]{background-color:rgba(255,255,68,33%);}"
		"*[bgColor=\"3\"]{background-color:rgba(68,255,68,33%);}"
		"*[bgColor=\"4\"]{background-color:rgba(68,255,255,33%);}"
		"*[bgColor=\"5\"]{background-color:rgba(68,68,255,33%);}"
		"*[bgColor=\"6\"]{background-color:rgba(255,68,255,33%);}"
		"*[bgColor=\"7\"]{background-color:rgba(68,68,68,33%);}"
		"*[bgColor=\"8\"]{background-color:rgba(255,255,255,33%);}"));

	obs_data_t *privData = obs_sceneitem_get_private_settings(item);
	obs_data_release(privData);

	obs_data_set_default_int(privData, "color-preset", 0);
	int preset = obs_data_get_int(privData, "color-preset");

	action = menu->addAction(QTStr("Clear"), this, +SLOT(ColorChange()));
	action->setCheckable(true);
	action->setProperty("bgColor", 0);
	action->setChecked(preset == 0);

	action = menu->addAction(QTStr("CustomColor"), this,
				 +SLOT(ColorChange()));
	action->setCheckable(true);
	action->setProperty("bgColor", 1);
	action->setChecked(preset == 1);

	menu->addSeparator();

	widgetAction->setDefaultWidget(select);

	for (int i = 1; i < 9; i++) {
		stringstream button;
		button << "preset" << i;
		QPushButton *colorButton =
			select->findChild<QPushButton *>(button.str().c_str());
		if (preset == i + 1)
			colorButton->setStyleSheet("border: 2px solid black");

		colorButton->setProperty("bgColor", i);
		select->connect(colorButton, SIGNAL(released()), this,
				SLOT(ColorChange()));
	}

	menu->addAction(widgetAction);

	return menu;
}

ColorSelect::ColorSelect(QWidget *parent)
	: QWidget(parent), ui(new Ui::ColorSelect)
{
	ui->setupUi(this);
}

void OBSBasic::CreateSourcePopupMenu(int idx, bool preview)
{
	QMenu popup(this);
	delete previewProjectorSource;
	delete sourceProjector;
	delete scaleFilteringMenu;
	delete colorMenu;
	delete colorWidgetAction;
	delete colorSelect;
	delete deinterlaceMenu;

	if (preview) {
		QAction *action = popup.addAction(
			QTStr("Basic.Main.PreviewConextMenu.Enable"), this,
			SLOT(TogglePreview()));
		action->setCheckable(true);
		action->setChecked(
			obs_display_enabled(ui->preview->GetDisplay()));
		if (IsPreviewProgramMode())
			action->setEnabled(false);

		//popup.addAction(ui->actionLockPreview);
		//popup.addMenu(ui->scalingMenu);

		previewProjectorSource = new QMenu(QTStr("PreviewProjector"));
		AddProjectorMenuMonitors(previewProjectorSource, this,
					 SLOT(OpenPreviewProjector()));

		popup.addMenu(previewProjectorSource);

		QAction *previewWindow =
			popup.addAction(QTStr("PreviewWindow"), this,
					SLOT(OpenPreviewWindow()));

		popup.addAction(previewWindow);

		popup.addSeparator();
	}

	QPointer<QMenu> addSourceMenu = CreateAddSourcePopupMenu();
	if (addSourceMenu)
		popup.addMenu(addSourceMenu);

	//ui->actionCopyFilters->setEnabled(false);
	//ui->actionCopySource->setEnabled(false);

	if (ui->sources->MultipleBaseSelected()) {
		popup.addSeparator();
		popup.addAction(QTStr("Basic.Main.GroupItems"), ui->sources,
				SLOT(GroupSelectedItems()));

	} else if (ui->sources->GroupsSelected()) {
		popup.addSeparator();
		popup.addAction(QTStr("Basic.Main.Ungroup"), ui->sources,
				SLOT(UngroupSelectedGroups()));
	}

	popup.addSeparator();
#if 0
	popup.addAction(ui->actionCopySource);
	popup.addAction(ui->actionPasteRef);
	popup.addAction(ui->actionPasteDup);
	popup.addSeparator();

	popup.addSeparator();
	popup.addAction(ui->actionCopyFilters);
	popup.addAction(ui->actionPasteFilters);
	popup.addSeparator();
#endif
	if (idx != -1) {
		if (addSourceMenu)
			popup.addSeparator();

		OBSSceneItem sceneItem = ui->sources->Get(idx);
		bool lock = obs_sceneitem_locked(sceneItem);
		obs_source_t *source = obs_sceneitem_get_source(sceneItem);
		uint32_t flags = obs_source_get_output_flags(source);
		bool isAsyncVideo = (flags & OBS_SOURCE_ASYNC_VIDEO) ==
				    OBS_SOURCE_ASYNC_VIDEO;
		bool hasAudio = (flags & OBS_SOURCE_AUDIO) == OBS_SOURCE_AUDIO;
		QAction *action;

		colorMenu = new QMenu(QTStr("ChangeBG"));
		colorWidgetAction = new QWidgetAction(colorMenu);
		colorSelect = new ColorSelect(colorMenu);
		popup.addMenu(AddBackgroundColorMenu(
			colorMenu, colorWidgetAction, colorSelect, sceneItem));
		popup.addAction(QTStr("Rename"), this,
				SLOT(EditSceneItemName()));
		popup.addAction(QTStr("Remove"), this,
				SLOT(on_actionRemoveSource_triggered()));
		popup.addSeparator();
#if 0
		popup.addMenu(ui->orderMenu);
		popup.addMenu(ui->transformMenu);

		ui->actionResetTransform->setEnabled(!lock);
		ui->actionRotate90CW->setEnabled(!lock);
		ui->actionRotate90CCW->setEnabled(!lock);
		ui->actionRotate180->setEnabled(!lock);
		ui->actionFlipHorizontal->setEnabled(!lock);
		ui->actionFlipVertical->setEnabled(!lock);
		ui->actionFitToScreen->setEnabled(!lock);
		ui->actionStretchToScreen->setEnabled(!lock);
		ui->actionCenterToScreen->setEnabled(!lock);
		ui->actionVerticalCenter->setEnabled(!lock);
		ui->actionHorizontalCenter->setEnabled(!lock);
#endif
		sourceProjector = new QMenu(QTStr("SourceProjector"));
		AddProjectorMenuMonitors(sourceProjector, this,
					 SLOT(OpenSourceProjector()));

		QAction *sourceWindow = popup.addAction(
			QTStr("SourceWindow"), this, SLOT(OpenSourceWindow()));

		popup.addAction(sourceWindow);

		popup.addSeparator();

		if (hasAudio) {
			QAction *actionHideMixer =
				popup.addAction(QTStr("HideMixer"), this,
						SLOT(ToggleHideMixer()));
			actionHideMixer->setCheckable(true);
			actionHideMixer->setChecked(SourceMixerHidden(source));
		}

		if (isAsyncVideo) {
			deinterlaceMenu = new QMenu(QTStr("Deinterlacing"));
			popup.addMenu(
				AddDeinterlacingMenu(deinterlaceMenu, source));
			popup.addSeparator();
		}

		QAction *resizeOutput =
			popup.addAction(QTStr("ResizeOutputSizeOfSource"), this,
					SLOT(ResizeOutputSizeOfSource()));

		int width = obs_source_get_width(source);
		int height = obs_source_get_height(source);

		resizeOutput->setEnabled(!obs_video_active());

		if (width == 0 || height == 0)
			resizeOutput->setEnabled(false);

		scaleFilteringMenu = new QMenu(QTStr("ScaleFiltering"));
		popup.addMenu(
			AddScaleFilteringMenu(scaleFilteringMenu, sceneItem));
		popup.addSeparator();

		popup.addMenu(sourceProjector);
		popup.addAction(sourceWindow);
		popup.addSeparator();

		action = popup.addAction(QTStr("Interact"), this,
					 SLOT(on_actionInteract_triggered()));

		action->setEnabled(obs_source_get_output_flags(source) &
				   OBS_SOURCE_INTERACTION);

		popup.addAction(QTStr("Filters"), this, SLOT(OpenFilters()));
		popup.addAction(QTStr("Properties"), this,
				SLOT(on_actionSourceProperties_triggered()));
#if 0
		ui->actionCopyFilters->setEnabled(true);
		ui->actionCopySource->setEnabled(true);
#endif
	} else {
		//ui->actionPasteFilters->setEnabled(false);
	}

	popup.exec(QCursor::pos());
}

void OBSBasic::on_sources_customContextMenuRequested(const QPoint &pos)
{
	if (ui->scenes->count()) {
		QModelIndex idx = ui->sources->indexAt(pos);
		CreateSourcePopupMenu(idx.row(), false);
	}
}

void OBSBasic::on_scenes_itemDoubleClicked(QListWidgetItem *witem)
{
	if (!witem)
		return;

	if (IsPreviewProgramMode()) {
		bool doubleClickSwitch =
			config_get_bool(App()->GlobalConfig(), "BasicWindow",
					"TransitionOnDoubleClick");

		if (doubleClickSwitch)
			TransitionClicked();
	}
}

void OBSBasic::AddSource(const char *id)
{
	if (id && *id) {
		OBSBasicSourceSelect sourceSelect(this, id);
		sourceSelect.exec();
		if (sourceSelect.newSource && strcmp(id, "group") != 0)
			CreatePropertiesWindow(sourceSelect.newSource);
	}
}

QMenu *OBSBasic::CreateAddSourcePopupMenu()
{
	const char *unversioned_type;
	const char *type;
	bool foundValues = false;
	bool foundDeprecated = false;
	size_t idx = 0;

	QMenu *popup = new QMenu(QTStr("Add"), this);
	QMenu *deprecated = new QMenu(QTStr("Deprecated"), popup);

	auto getActionAfter = [](QMenu *menu, const QString &name) {
		QList<QAction *> actions = menu->actions();

		for (QAction *menuAction : actions) {
			if (menuAction->text().compare(name) >= 0)
				return menuAction;
		}

		return (QAction *)nullptr;
	};

	auto addSource = [this, getActionAfter](QMenu *popup, const char *type,
						const char *name) {
		QString qname = QT_UTF8(name);
		QAction *popupItem = new QAction(qname, this);
		popupItem->setData(QT_UTF8(type));
		connect(popupItem, SIGNAL(triggered(bool)), this,
			SLOT(AddSourceFromAction()));

		QIcon icon;

		if (strcmp(type, "scene") == 0)
			icon = GetSceneIcon();
		else
			icon = GetSourceIcon(type);

		popupItem->setIcon(icon);

		QAction *after = getActionAfter(popup, qname);
		popup->insertAction(after, popupItem);
	};

	while (obs_enum_input_types2(idx++, &type, &unversioned_type)) {
		const char *name = obs_source_get_display_name(type);
		uint32_t caps = obs_get_source_output_flags(type);

		if ((caps & OBS_SOURCE_CAP_DISABLED) != 0)
			continue;

		if ((caps & OBS_SOURCE_DEPRECATED) == 0) {
			addSource(popup, unversioned_type, name);
		} else {
			addSource(deprecated, unversioned_type, name);
			foundDeprecated = true;
		}
		foundValues = true;
	}

	addSource(popup, "scene", Str("Basic.Scene"));

	popup->addSeparator();
	QAction *addGroup = new QAction(QTStr("Group"), this);
	addGroup->setData(QT_UTF8("group"));
	addGroup->setIcon(GetGroupIcon());
	connect(addGroup, SIGNAL(triggered(bool)), this,
		SLOT(AddSourceFromAction()));
	popup->addAction(addGroup);

	if (!foundDeprecated) {
		delete deprecated;
		deprecated = nullptr;
	}

	if (!foundValues) {
		delete popup;
		popup = nullptr;

	} else if (foundDeprecated) {
		popup->addSeparator();
		popup->addMenu(deprecated);
	}

	return popup;
}

void OBSBasic::AddSourceFromAction()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (!action)
		return;

	AddSource(QT_TO_UTF8(action->data().toString()));
}

void OBSBasic::AddSourcePopupMenu(const QPoint &pos)
{
	if (!GetCurrentScene()) {
		// Tell the user he needs a scene first (help beginners).
		OBSMessageBox::information(
			this, QTStr("Basic.Main.AddSourceHelp.Title"),
			QTStr("Basic.Main.AddSourceHelp.Text"));
		return;
	}

	QScopedPointer<QMenu> popup(CreateAddSourcePopupMenu());
	if (popup)
		popup->exec(pos);
}

static void MyAddSource(void *_data, obs_scene_t *scene)
{
	AddSourceData *data = (AddSourceData *)_data;
	obs_sceneitem_t *sceneitem;

	sceneitem = obs_scene_add(scene, data->source);
	obs_sceneitem_set_visible(sceneitem, data->visible);
}

void OBSBasic::on_actionAddSource_triggered()
{
	AddSourcePopupMenu(QCursor::pos());
}

static bool remove_items(obs_scene_t *, obs_sceneitem_t *item, void *param)
{
	vector<OBSSceneItem> &items =
		*reinterpret_cast<vector<OBSSceneItem> *>(param);

	if (obs_sceneitem_selected(item)) {
		items.emplace_back(item);
	} else if (obs_sceneitem_is_group(item)) {
		obs_sceneitem_group_enum_items(item, remove_items, &items);
	}
	return true;
};

void OBSBasic::on_actionRemoveSource_triggered()
{
	vector<OBSSceneItem> items;

	obs_scene_enum_items(GetCurrentScene(), remove_items, &items);

	if (!items.size())
		return;

	auto removeMultiple = [this](size_t count) {
		QString text = QTStr("ConfirmRemove.TextMultiple")
				       .arg(QString::number(count));

		QMessageBox remove_items(this);
		remove_items.setText(text);
		QAbstractButton *Yes = remove_items.addButton(
			QTStr("Yes"), QMessageBox::YesRole);
		remove_items.addButton(QTStr("No"), QMessageBox::NoRole);
		remove_items.setIcon(QMessageBox::Question);
		remove_items.setWindowTitle(QTStr("ConfirmRemove.Title"));
		remove_items.exec();

		return Yes == remove_items.clickedButton();
	};

	if (items.size() == 1) {
		OBSSceneItem &item = items[0];
		obs_source_t *source = obs_sceneitem_get_source(item);

		if (source && QueryRemoveSource(source))
			obs_sceneitem_remove(item);
	} else {
		if (removeMultiple(items.size())) {
			for (auto &item : items)
				obs_sceneitem_remove(item);
		}
	}
}

void OBSBasic::on_actionInteract_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	OBSSource source = obs_sceneitem_get_source(item);

	if (source)
		CreateInteractionWindow(source);
}

void OBSBasic::on_actionSourceProperties_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	OBSSource source = obs_sceneitem_get_source(item);

	if (source)
		CreatePropertiesWindow(source);
}

void OBSBasic::on_actionSourceUp_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_UP);
}

void OBSBasic::on_actionSourceDown_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_DOWN);
}

void OBSBasic::on_actionMoveUp_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_UP);
}

void OBSBasic::on_actionMoveDown_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_DOWN);
}

void OBSBasic::on_actionMoveToTop_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_TOP);
}

void OBSBasic::on_actionMoveToBottom_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();
	obs_sceneitem_set_order(item, OBS_ORDER_MOVE_BOTTOM);
}

static BPtr<char> ReadLogFile(const char *subdir, const char *log)
{
	char logDir[512];
	if (GetConfigPath(logDir, sizeof(logDir), subdir) <= 0)
		return nullptr;

	string path = logDir;
	path += "/";
	path += log;

	BPtr<char> file = os_quick_read_utf8_file(path.c_str());
	if (!file)
		blog(LOG_WARNING, "Failed to read log file %s", path.c_str());

	return file;
}

void OBSBasic::UploadLog(const char *subdir, const char *file)
{
	BPtr<char> fileString{ReadLogFile(subdir, file)};

	if (!fileString)
		return;

	if (!*fileString)
		return;

	//ui->menuLogFiles->setEnabled(false);

	stringstream ss;
	ss << "OBS " << App()->GetVersionString() << " log file uploaded at "
	   << CurrentDateTimeString() << "\n\n"
	   << fileString;

	if (logUploadThread) {
		logUploadThread->wait();
	}

	RemoteTextThread *thread =
		new RemoteTextThread("https://obsproject.com/logs/upload",
				     "text/plain", ss.str().c_str());

	logUploadThread.reset(thread);
	connect(thread, &RemoteTextThread::Result, this,
		&OBSBasic::logUploadFinished);
	logUploadThread->start();
}

void OBSBasic::on_actionShowLogs_triggered()
{
	char logDir[512];
	if (GetConfigPath(logDir, sizeof(logDir), "obs-studio/logs") <= 0)
		return;

	QUrl url = QUrl::fromLocalFile(QT_UTF8(logDir));
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionUploadCurrentLog_triggered()
{
	UploadLog("obs-studio/logs", App()->GetCurrentLog());
}

void OBSBasic::on_actionUploadLastLog_triggered()
{
	UploadLog("obs-studio/logs", App()->GetLastLog());
}

void OBSBasic::on_actionViewCurrentLog_triggered()
{
	char logDir[512];
	if (GetConfigPath(logDir, sizeof(logDir), "obs-studio/logs") <= 0)
		return;

	const char *log = App()->GetCurrentLog();

	string path = logDir;
	path += "/";
	path += log;

	QUrl url = QUrl::fromLocalFile(QT_UTF8(path.c_str()));
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionShowCrashLogs_triggered()
{
	char logDir[512];
	if (GetConfigPath(logDir, sizeof(logDir), "obs-studio/crashes") <= 0)
		return;

	QUrl url = QUrl::fromLocalFile(QT_UTF8(logDir));
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionUploadLastCrashLog_triggered()
{
	UploadLog("obs-studio/crashes", App()->GetLastCrashLog());
}

void OBSBasic::on_actionCheckForUpdates_triggered()
{
	CheckForUpdates(true);
}

void OBSBasic::logUploadFinished(const QString &text, const QString &error)
{
	//ui->menuLogFiles->setEnabled(true);

	if (text.isEmpty()) {
		OBSMessageBox::critical(
			this, QTStr("LogReturnDialog.ErrorUploadingLog"),
			error);
		return;
	}

	obs_data_t *returnData = obs_data_create_from_json(QT_TO_UTF8(text));
	string resURL = obs_data_get_string(returnData, "url");
	QString logURL = resURL.c_str();
	obs_data_release(returnData);

	OBSLogReply logDialog(this, logURL);
	logDialog.exec();
}

static void RenameListItem(OBSBasic *parent, QListWidget *listWidget,
			   obs_source_t *source, const string &name)
{
	const char *prevName = obs_source_get_name(source);
	if (name == prevName)
		return;

	obs_source_t *foundSource = obs_get_source_by_name(name.c_str());
	QListWidgetItem *listItem = listWidget->currentItem();

	if (foundSource || name.empty()) {
		listItem->setText(QT_UTF8(prevName));

		if (foundSource) {
			OBSMessageBox::warning(parent,
					       QTStr("NameExists.Title"),
					       QTStr("NameExists.Text"));
		} else if (name.empty()) {
			OBSMessageBox::warning(parent,
					       QTStr("NoNameEntered.Title"),
					       QTStr("NoNameEntered.Text"));
		}

		obs_source_release(foundSource);
	} else {
		listItem->setText(QT_UTF8(name.c_str()));
		obs_source_set_name(source, name.c_str());
	}
}

void OBSBasic::SceneNameEdited(QWidget *editor,
			       QAbstractItemDelegate::EndEditHint endHint)
{
	OBSScene scene = GetCurrentScene();
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	string text = QT_TO_UTF8(edit->text().trimmed());

	if (!scene)
		return;

	obs_source_t *source = obs_scene_get_source(scene);
	RenameListItem(this, ui->scenes, source, text);

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_SCENE_LIST_CHANGED);

	UNUSED_PARAMETER(endHint);
}

void OBSBasic::OpenFilters()
{
	OBSSceneItem item = GetCurrentSceneItem();
	OBSSource source = obs_sceneitem_get_source(item);

	CreateFiltersWindow(source);
}

void OBSBasic::OpenSceneFilters()
{
	OBSScene scene = GetCurrentScene();
	OBSSource source = obs_scene_get_source(scene);

	CreateFiltersWindow(source);
}

#define RECORDING_START \
	"==== Recording Start ==============================================="
#define RECORDING_STOP \
	"==== Recording Stop ================================================"
#define REPLAY_BUFFER_START \
	"==== Replay Buffer Start ==========================================="
#define REPLAY_BUFFER_STOP \
	"==== Replay Buffer Stop ============================================"
#define STREAMING_START \
	"==== Streaming Start ==============================================="
#define STREAMING_STOP \
	"==== Streaming Stop ================================================"
#if 0
void OBSBasic::StartStreaming()
{
	if (outputHandler->StreamingActive())
		return;
	if (disableOutputsRef)
		return;

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_STREAMING_STARTING);

	SaveProject();

	ui->streamButton->setEnabled(false);
	ui->streamButton->setText(QTStr("Basic.Main.Connecting"));

	if (sysTrayStream) {
		sysTrayStream->setEnabled(false);
		sysTrayStream->setText(ui->streamButton->text());
	}

	if (!outputHandler->StartStreaming(service)) {
		QString message =
			!outputHandler->lastError.empty()
				? QTStr(outputHandler->lastError.c_str())
				: QTStr("Output.StartFailedGeneric");
		ui->streamButton->setText(QTStr("Basic.Main.StartStreaming"));
		ui->streamButton->setEnabled(true);
		ui->streamButton->setChecked(false);

		if (sysTrayStream) {
			sysTrayStream->setText(ui->streamButton->text());
			sysTrayStream->setEnabled(true);
		}

		QMessageBox::critical(this, QTStr("Output.StartStreamFailed"),
				      message);
		return;
	}

	bool recordWhenStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "RecordWhenStreaming");
	if (recordWhenStreaming)
		StartRecording();

	bool replayBufferWhileStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "ReplayBufferWhileStreaming");
	if (replayBufferWhileStreaming)
		StartReplayBuffer();
}
#endif
#ifdef _WIN32
static inline void UpdateProcessPriority()
{
	const char *priority = config_get_string(App()->GlobalConfig(),
						 "General", "ProcessPriority");
	if (priority && strcmp(priority, "Normal") != 0)
		SetProcessPriority(priority);
}

static inline void ClearProcessPriority()
{
	const char *priority = config_get_string(App()->GlobalConfig(),
						 "General", "ProcessPriority");
	if (priority && strcmp(priority, "Normal") != 0)
		SetProcessPriority("Normal");
}
#else
#define UpdateProcessPriority() \
	do {                    \
	} while (false)
#define ClearProcessPriority() \
	do {                   \
	} while (false)
#endif

inline void OBSBasic::OnActivate()
{
#if 0
	if (ui->profileMenu->isEnabled()) {
		ui->profileMenu->setEnabled(false);
		ui->autoConfigure->setEnabled(false);
		App()->IncrementSleepInhibition();
		UpdateProcessPriority();

		if (trayIcon)
			trayIcon->setIcon(QIcon::fromTheme(
				"obs-tray-active",
				QIcon(":/res/images/tray_active.png")));
	}
#endif
}

extern volatile bool recording_paused;
extern volatile bool replaybuf_active;

inline void OBSBasic::OnDeactivate()
{
#if 0
	if (!outputHandler->Active() && !ui->profileMenu->isEnabled()) {
		ui->profileMenu->setEnabled(true);
		ui->autoConfigure->setEnabled(true);
		App()->DecrementSleepInhibition();
		ClearProcessPriority();

		if (trayIcon)
			trayIcon->setIcon(QIcon::fromTheme(
				"obs-tray", QIcon(":/res/images/obs.png")));
	} else if (trayIcon) {
		if (os_atomic_load_bool(&recording_paused))
			trayIcon->setIcon(QIcon(":/res/images/obs_paused.png"));
		else
			trayIcon->setIcon(
				QIcon(":/res/images/tray_active.png"));
	}
#endif
}
#if 0
void OBSBasic::StopStreaming()
{
	SaveProject();

	if (outputHandler->StreamingActive())
		outputHandler->StopStreaming(streamingStopping);

	OnDeactivate();

	bool recordWhenStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "RecordWhenStreaming");
	bool keepRecordingWhenStreamStops =
		config_get_bool(GetGlobalConfig(), "BasicWindow",
				"KeepRecordingWhenStreamStops");
	if (recordWhenStreaming && !keepRecordingWhenStreamStops)
		StopRecording();

	bool replayBufferWhileStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "ReplayBufferWhileStreaming");
	bool keepReplayBufferStreamStops =
		config_get_bool(GetGlobalConfig(), "BasicWindow",
				"KeepReplayBufferStreamStops");
	if (replayBufferWhileStreaming && !keepReplayBufferStreamStops)
		StopReplayBuffer();
}

void OBSBasic::ForceStopStreaming()
{
	SaveProject();

	if (outputHandler->StreamingActive())
		outputHandler->StopStreaming(true);

	OnDeactivate();

	bool recordWhenStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "RecordWhenStreaming");
	bool keepRecordingWhenStreamStops =
		config_get_bool(GetGlobalConfig(), "BasicWindow",
				"KeepRecordingWhenStreamStops");
	if (recordWhenStreaming && !keepRecordingWhenStreamStops)
		StopRecording();

	bool replayBufferWhileStreaming = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "ReplayBufferWhileStreaming");
	bool keepReplayBufferStreamStops =
		config_get_bool(GetGlobalConfig(), "BasicWindow",
				"KeepReplayBufferStreamStops");
	if (replayBufferWhileStreaming && !keepReplayBufferStreamStops)
		StopReplayBuffer();
}

void OBSBasic::StreamDelayStarting(int sec)
{
	ui->streamButton->setText(QTStr("Basic.Main.StopStreaming"));
	ui->streamButton->setEnabled(true);
	ui->streamButton->setChecked(true);

	if (sysTrayStream) {
		sysTrayStream->setText(ui->streamButton->text());
		sysTrayStream->setEnabled(true);
	}

	if (!startStreamMenu.isNull())
		startStreamMenu->deleteLater();

	startStreamMenu = new QMenu();
	startStreamMenu->addAction(QTStr("Basic.Main.StopStreaming"), this,
				   SLOT(StopStreaming()));
	startStreamMenu->addAction(QTStr("Basic.Main.ForceStopStreaming"), this,
				   SLOT(ForceStopStreaming()));
	ui->streamButton->setMenu(startStreamMenu);

	ui->statusbar->StreamDelayStarting(sec);

	OnActivate();
}

void OBSBasic::StreamDelayStopping(int sec)
{
	ui->streamButton->setText(QTStr("Basic.Main.StartStreaming"));
	ui->streamButton->setEnabled(true);
	ui->streamButton->setChecked(false);

	if (sysTrayStream) {
		sysTrayStream->setText(ui->streamButton->text());
		sysTrayStream->setEnabled(true);
	}

	if (!startStreamMenu.isNull())
		startStreamMenu->deleteLater();

	startStreamMenu = new QMenu();
	startStreamMenu->addAction(QTStr("Basic.Main.StartStreaming"), this,
				   SLOT(StartStreaming()));
	startStreamMenu->addAction(QTStr("Basic.Main.ForceStopStreaming"), this,
				   SLOT(ForceStopStreaming()));
	ui->streamButton->setMenu(startStreamMenu);

	ui->statusbar->StreamDelayStopping(sec);

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_STREAMING_STOPPING);
}

void OBSBasic::StreamingStart()
{
	ui->streamButton->setText(QTStr("Basic.Main.StopStreaming"));
	ui->streamButton->setEnabled(true);
	ui->streamButton->setChecked(true);
	ui->statusbar->StreamStarted(outputHandler->streamOutput);

	if (sysTrayStream) {
		sysTrayStream->setText(ui->streamButton->text());
		sysTrayStream->setEnabled(true);
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_STREAMING_STARTED);

	OnActivate();

	blog(LOG_INFO, STREAMING_START);
}

void OBSBasic::StreamStopping()
{
	ui->streamButton->setText(QTStr("Basic.Main.StoppingStreaming"));

	if (sysTrayStream)
		sysTrayStream->setText(ui->streamButton->text());

	streamingStopping = true;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_STREAMING_STOPPING);
}

void OBSBasic::StreamingStop(int code, QString last_error)
{
	const char *errorDescription = "";
	DStr errorMessage;
	bool use_last_error = false;
	bool encode_error = false;

	switch (code) {
	case OBS_OUTPUT_BAD_PATH:
		errorDescription = Str("Output.ConnectFail.BadPath");
		break;

	case OBS_OUTPUT_CONNECT_FAILED:
		use_last_error = true;
		errorDescription = Str("Output.ConnectFail.ConnectFailed");
		break;

	case OBS_OUTPUT_INVALID_STREAM:
		errorDescription = Str("Output.ConnectFail.InvalidStream");
		break;

	case OBS_OUTPUT_ENCODE_ERROR:
		encode_error = true;
		break;

	default:
	case OBS_OUTPUT_ERROR:
		use_last_error = true;
		errorDescription = Str("Output.ConnectFail.Error");
		break;

	case OBS_OUTPUT_DISCONNECTED:
		/* doesn't happen if output is set to reconnect.  note that
		 * reconnects are handled in the output, not in the UI */
		use_last_error = true;
		errorDescription = Str("Output.ConnectFail.Disconnected");
	}

	if (use_last_error && !last_error.isEmpty())
		dstr_printf(errorMessage, "%s\n\n%s", errorDescription,
			    QT_TO_UTF8(last_error));
	else
		dstr_copy(errorMessage, errorDescription);
#if 0
	ui->statusbar->StreamStopped();
#endif
	ui->streamButton->setText(QTStr("Basic.Main.StartStreaming"));
	ui->streamButton->setEnabled(true);
	ui->streamButton->setChecked(false);

	if (sysTrayStream) {
		sysTrayStream->setText(ui->streamButton->text());
		sysTrayStream->setEnabled(true);
	}

	streamingStopping = false;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_STREAMING_STOPPED);

	OnDeactivate();

	blog(LOG_INFO, STREAMING_STOP);

	if (encode_error) {
		OBSMessageBox::information(
			this, QTStr("Output.StreamEncodeError.Title"),
			QTStr("Output.StreamEncodeError.Msg"));

	} else if (code != OBS_OUTPUT_SUCCESS && isVisible()) {
		OBSMessageBox::information(this,
					   QTStr("Output.ConnectFail.Title"),
					   QT_UTF8(errorMessage));

	} else if (code != OBS_OUTPUT_SUCCESS && !isVisible()) {
		SysTrayNotify(QT_UTF8(errorDescription),
			      QSystemTrayIcon::Warning);
	}

	if (!startStreamMenu.isNull()) {
		ui->streamButton->setMenu(nullptr);
		startStreamMenu->deleteLater();
		startStreamMenu = nullptr;
	}
}
#endif
void OBSBasic::AutoRemux()
{
	const char *mode = config_get_string(basicConfig, "Output", "Mode");
	bool advanced = astrcmpi(mode, "Advanced") == 0;

	const char *path = !advanced ? config_get_string(basicConfig,
							 "SimpleOutput",
							 "FilePath")
				     : config_get_string(basicConfig, "AdvOut",
							 "RecFilePath");

	/* do not save if using FFmpeg output in advanced output mode */
	if (advanced) {
		const char *type =
			config_get_string(basicConfig, "AdvOut", "RecType");
		if (astrcmpi(type, "FFmpeg") == 0) {
			return;
		}
	}

	QString input;
	input += path;
	input += "/";
	input += remuxFilename.c_str();

	QFileInfo fi(remuxFilename.c_str());

	/* do not remux if lossless */
	if (fi.suffix().compare("avi", Qt::CaseInsensitive) == 0) {
		return;
	}

	QString output;
	output += path;
	output += "/";
	output += fi.completeBaseName();
	output += ".mp4";

	OBSRemux *remux = new OBSRemux(path, this, true);
	remux->show();
	remux->AutoRemux(input, output);
}

void OBSBasic::StartRecording()
{
	if (outputHandler->RecordingActive())
		return;
	if (disableOutputsRef)
		return;

	if (!OutputPathValid()) {
		OutputPathInvalidMessage();
		ui->recordButton->setChecked(false);
		return;
	}

	if (LowDiskSpace()) {
		DiskSpaceMessage();
		ui->recordButton->setChecked(false);
		return;
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_RECORDING_STARTING);

	SaveProject();

	if (!outputHandler->StartRecording())
		ui->recordButton->setChecked(false);
}

void OBSBasic::RecordStopping()
{
	ui->recordButton->setText(QTStr("Basic.Main.StoppingRecording"));

	if (sysTrayRecord)
		sysTrayRecord->setText(ui->recordButton->text());

	recordingStopping = true;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_RECORDING_STOPPING);
}

void OBSBasic::StopRecording()
{
	SaveProject();

	if (outputHandler->RecordingActive())
		outputHandler->StopRecording(recordingStopping);

	OnDeactivate();
}

void OBSBasic::RecordingStart()
{
	//ui->statusbar->RecordingStarted(outputHandler->fileOutput);
	if (!active) {
		refreshTimer = new QTimer(this);
		//connect(refreshTimer, SIGNAL(timeout()), this, SLOT(UpdateStatusBar()));
		connect(refreshTimer, SIGNAL(timeout()), this, SLOT(UpdateRecordTime()));
		totalRecordSeconds = 0;
		refreshTimer->start(1000);
		active = true;
    }
	ui->recordButton->setText(QTStr("Basic.Main.StopRecording"));
	ui->recordButton->setChecked(true);

	if (sysTrayRecord)
		sysTrayRecord->setText(ui->recordButton->text());

	recordingStopping = false;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_RECORDING_STARTED);
#if 0
	if (!diskFullTimer->isActive())
		diskFullTimer->start(1000);
#endif
	OnActivate();
	UpdatePause();

	blog(LOG_INFO, RECORDING_START);
}

void OBSBasic::RecordingStop(int code, QString last_error)
{
	// ui->statusbar->RecordingStopped();
    {
        recordTime->setText(QString("REC: 00:00:00"));
        totalRecordSeconds = 0;
        if (!outputHandler || !outputHandler->Active()) {
            if(refreshTimer)
                delete refreshTimer;
            active = false;
        }
    }
	ui->recordButton->setText(QTStr("Basic.Main.StartRecording"));
	ui->recordButton->setChecked(false);

	if (sysTrayRecord)
		sysTrayRecord->setText(ui->recordButton->text());

	blog(LOG_INFO, RECORDING_STOP);

	if (code == OBS_OUTPUT_UNSUPPORTED && isVisible()) {
		OBSMessageBox::critical(this, QTStr("Output.RecordFail.Title"),
					QTStr("Output.RecordFail.Unsupported"));

	} else if (code == OBS_OUTPUT_ENCODE_ERROR && isVisible()) {
		OBSMessageBox::warning(
			this, QTStr("Output.RecordError.Title"),
			QTStr("Output.RecordError.EncodeErrorMsg"));

	} else if (code == OBS_OUTPUT_NO_SPACE && isVisible()) {
		OBSMessageBox::warning(this,
				       QTStr("Output.RecordNoSpace.Title"),
				       QTStr("Output.RecordNoSpace.Msg"));

	} else if (code != OBS_OUTPUT_SUCCESS && isVisible()) {

		const char *errorDescription;
		DStr errorMessage;
		bool use_last_error = true;

		errorDescription = Str("Output.RecordError.Msg");

		if (use_last_error && !last_error.isEmpty())
			dstr_printf(errorMessage, "%s\n\n%s", errorDescription,
				    QT_TO_UTF8(last_error));
		else
			dstr_copy(errorMessage, errorDescription);

		OBSMessageBox::critical(this, QTStr("Output.RecordError.Title"),
					QT_UTF8(errorMessage));

	} else if (code == OBS_OUTPUT_UNSUPPORTED && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordFail.Unsupported"),
			      QSystemTrayIcon::Warning);

	} else if (code == OBS_OUTPUT_NO_SPACE && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordNoSpace.Msg"),
			      QSystemTrayIcon::Warning);

	} else if (code != OBS_OUTPUT_SUCCESS && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordError.Msg"),
			      QSystemTrayIcon::Warning);
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_RECORDING_STOPPED);
#if 0
	if (diskFullTimer->isActive())
		diskFullTimer->stop();
#endif
	if (remuxAfterRecord)
		AutoRemux();

	OnDeactivate();
	UpdatePause(false);
}

#define RP_NO_HOTKEY_TITLE QTStr("Output.ReplayBuffer.NoHotkey.Title")
#define RP_NO_HOTKEY_TEXT QTStr("Output.ReplayBuffer.NoHotkey.Msg")

void OBSBasic::ShowReplayBufferPauseWarning()
{
	auto msgBox = []() {
		QMessageBox msgbox(App()->GetMainWindow());
		msgbox.setWindowTitle(QTStr("Output.ReplayBuffer."
					    "PauseWarning.Title"));
		msgbox.setText(QTStr("Output.ReplayBuffer."
				     "PauseWarning.Text"));
		msgbox.setIcon(QMessageBox::Icon::Information);
		msgbox.addButton(QMessageBox::Ok);

		QCheckBox *cb = new QCheckBox(QTStr("DoNotShowAgain"));
		msgbox.setCheckBox(cb);

		msgbox.exec();

		if (cb->isChecked()) {
			config_set_bool(App()->GlobalConfig(), "General",
					"WarnedAboutReplayBufferPausing", true);
			config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
		}
	};

	bool warned = config_get_bool(App()->GlobalConfig(), "General",
				      "WarnedAboutReplayBufferPausing");
	if (!warned) {
		QMetaObject::invokeMethod(App(), "Exec", Qt::QueuedConnection,
					  Q_ARG(VoidFunc, msgBox));
	}
}

void OBSBasic::StartReplayBuffer()
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;
	if (outputHandler->ReplayBufferActive())
		return;
	if (disableOutputsRef)
		return;

	if (!UIValidation::NoSourcesConfirmation(this)) {
		replayBufferButton->setChecked(false);
		return;
	}

	if (LowDiskSpace()) {
		DiskSpaceMessage();
		replayBufferButton->setChecked(false);
		return;
	}

	obs_output_t *output = outputHandler->replayBuffer;
	obs_data_t *hotkeys = obs_hotkeys_save_output(output);
	obs_data_array_t *bindings =
		obs_data_get_array(hotkeys, "ReplayBuffer.Save");
	size_t count = obs_data_array_count(bindings);
	obs_data_array_release(bindings);
	obs_data_release(hotkeys);

	if (!count) {
		OBSMessageBox::information(this, RP_NO_HOTKEY_TITLE,
					   RP_NO_HOTKEY_TEXT);
		replayBufferButton->setChecked(false);
		return;
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_REPLAY_BUFFER_STARTING);

	SaveProject();

	if (!outputHandler->StartReplayBuffer()) {
		replayBufferButton->setChecked(false);
	} else if (os_atomic_load_bool(&recording_paused)) {
		ShowReplayBufferPauseWarning();
	}
}

void OBSBasic::ReplayBufferStopping()
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;

	replayBufferButton->setText(QTStr("Basic.Main.StoppingReplayBuffer"));

	if (sysTrayReplayBuffer)
		sysTrayReplayBuffer->setText(replayBufferButton->text());

	replayBufferStopping = true;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_REPLAY_BUFFER_STOPPING);
}

void OBSBasic::StopReplayBuffer()
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;

	SaveProject();

	if (outputHandler->ReplayBufferActive())
		outputHandler->StopReplayBuffer(replayBufferStopping);

	OnDeactivate();
}

void OBSBasic::ReplayBufferStart()
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;

	replayBufferButton->setText(QTStr("Basic.Main.StopReplayBuffer"));
	replayBufferButton->setChecked(true);

	if (sysTrayReplayBuffer)
		sysTrayReplayBuffer->setText(replayBufferButton->text());

	replayBufferStopping = false;
	if (api)
		api->on_event(OBS_FRONTEND_EVENT_REPLAY_BUFFER_STARTED);

	OnActivate();
	UpdateReplayBuffer();

	blog(LOG_INFO, REPLAY_BUFFER_START);
}

void OBSBasic::ReplayBufferSave()
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;
	if (!outputHandler->ReplayBufferActive())
		return;

	calldata_t cd = {0};
	proc_handler_t *ph =
		obs_output_get_proc_handler(outputHandler->replayBuffer);
	proc_handler_call(ph, "save", &cd);
	calldata_free(&cd);
}

void OBSBasic::ReplayBufferStop(int code)
{
	if (!outputHandler || !outputHandler->replayBuffer)
		return;

	replayBufferButton->setText(QTStr("Basic.Main.StartReplayBuffer"));
	replayBufferButton->setChecked(false);

	if (sysTrayReplayBuffer)
		sysTrayReplayBuffer->setText(replayBufferButton->text());

	blog(LOG_INFO, REPLAY_BUFFER_STOP);

	if (code == OBS_OUTPUT_UNSUPPORTED && isVisible()) {
		OBSMessageBox::critical(this, QTStr("Output.RecordFail.Title"),
					QTStr("Output.RecordFail.Unsupported"));

	} else if (code == OBS_OUTPUT_NO_SPACE && isVisible()) {
		OBSMessageBox::warning(this,
				       QTStr("Output.RecordNoSpace.Title"),
				       QTStr("Output.RecordNoSpace.Msg"));

	} else if (code != OBS_OUTPUT_SUCCESS && isVisible()) {
		OBSMessageBox::critical(this, QTStr("Output.RecordError.Title"),
					QTStr("Output.RecordError.Msg"));

	} else if (code == OBS_OUTPUT_UNSUPPORTED && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordFail.Unsupported"),
			      QSystemTrayIcon::Warning);

	} else if (code == OBS_OUTPUT_NO_SPACE && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordNoSpace.Msg"),
			      QSystemTrayIcon::Warning);

	} else if (code != OBS_OUTPUT_SUCCESS && !isVisible()) {
		SysTrayNotify(QTStr("Output.RecordError.Msg"),
			      QSystemTrayIcon::Warning);
	}

	if (api)
		api->on_event(OBS_FRONTEND_EVENT_REPLAY_BUFFER_STOPPED);

	OnDeactivate();
	UpdateReplayBuffer(false);
}
#if 0
void OBSBasic::on_streamButton_clicked()
{
	if (outputHandler->StreamingActive()) {
		bool confirm = config_get_bool(GetGlobalConfig(), "BasicWindow",
					       "WarnBeforeStoppingStream");

		if (confirm && isVisible()) {
			QMessageBox::StandardButton button =
				OBSMessageBox::question(
					this, QTStr("ConfirmStop.Title"),
					QTStr("ConfirmStop.Text"),
					QMessageBox::Yes | QMessageBox::No,
					QMessageBox::No);

			if (button == QMessageBox::No) {
				ui->streamButton->setChecked(true);
				return;
			}
		}

		StopStreaming();
	} else {
		if (!UIValidation::NoSourcesConfirmation(this)) {
			ui->streamButton->setChecked(false);
			return;
		}

		auto action =
			UIValidation::StreamSettingsConfirmation(this, service);
		switch (action) {
		case StreamSettingsAction::ContinueStream:
			break;
		case StreamSettingsAction::OpenSettings:
			on_action_Settings_triggered();
			ui->streamButton->setChecked(false);
			return;
		case StreamSettingsAction::Cancel:
			ui->streamButton->setChecked(false);
			return;
		}

		bool confirm = config_get_bool(GetGlobalConfig(), "BasicWindow",
					       "WarnBeforeStartingStream");

		obs_data_t *settings = obs_service_get_settings(service);
		bool bwtest = obs_data_get_bool(settings, "bwtest");
		obs_data_release(settings);

		if (bwtest && isVisible()) {
			QMessageBox::StandardButton button =
				OBSMessageBox::question(
					this, QTStr("ConfirmBWTest.Title"),
					QTStr("ConfirmBWTest.Text"));

			if (button == QMessageBox::No) {
				ui->streamButton->setChecked(false);
				return;
			}
		} else if (confirm && isVisible()) {
			QMessageBox::StandardButton button =
				OBSMessageBox::question(
					this, QTStr("ConfirmStart.Title"),
					QTStr("ConfirmStart.Text"),
					QMessageBox::Yes | QMessageBox::No,
					QMessageBox::No);

			if (button == QMessageBox::No) {
				ui->streamButton->setChecked(false);
				return;
			}
		}

		StartStreaming();
	}
}
#endif
void OBSBasic::on_recordButton_clicked()
{
	if (outputHandler->RecordingActive()) {
		bool confirm = config_get_bool(GetGlobalConfig(), "BasicWindow",
					       "WarnBeforeStoppingRecord");

		if (confirm && isVisible()) {
			QMessageBox::StandardButton button =
				OBSMessageBox::question(
					this, QTStr("ConfirmStopRecord.Title"),
					QTStr("ConfirmStopRecord.Text"),
					QMessageBox::Yes | QMessageBox::No,
					QMessageBox::No);

			if (button == QMessageBox::No) {
				ui->recordButton->setChecked(true);
				return;
			}
		}
		StopRecording();
	} else {
		if (!UIValidation::NoSourcesConfirmation(this)) {
			ui->recordButton->setChecked(false);
			return;
		}

		StartRecording();
	}
}

void OBSBasic::on_settingsButton_clicked()
{
	on_action_Settings_triggered();
}

void OBSBasic::on_actionHelpPortal_triggered()
{
	QUrl url = QUrl("https://obsproject.com/help", QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionWebsite_triggered()
{
	QUrl url = QUrl("https://obsproject.com", QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionDiscord_triggered()
{
	QUrl url = QUrl("https://obsproject.com/discord", QUrl::TolerantMode);
	QDesktopServices::openUrl(url);
}

void OBSBasic::on_actionShowSettingsFolder_triggered()
{
	char path[512];
	int ret = GetConfigPath(path, 512, "obs-studio");
	if (ret <= 0)
		return;

	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void OBSBasic::on_actionShowProfileFolder_triggered()
{
	char path[512];
	int ret = GetProfilePath(path, 512, "");
	if (ret <= 0)
		return;

	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

int OBSBasic::GetTopSelectedSourceItem()
{
	QModelIndexList selectedItems =
		ui->sources->selectionModel()->selectedIndexes();
	return selectedItems.count() ? selectedItems[0].row() : -1;
}

QModelIndexList OBSBasic::GetAllSelectedSourceItems()
{
	return ui->sources->selectionModel()->selectedIndexes();
}

void OBSBasic::on_preview_customContextMenuRequested(const QPoint &pos)
{
	CreateSourcePopupMenu(GetTopSelectedSourceItem(), true);

	UNUSED_PARAMETER(pos);
}

void OBSBasic::on_program_customContextMenuRequested(const QPoint &)
{
	QMenu popup(this);
	QPointer<QMenu> studioProgramProjector;

	studioProgramProjector = new QMenu(QTStr("StudioProgramProjector"));
	AddProjectorMenuMonitors(studioProgramProjector, this,
				 SLOT(OpenStudioProgramProjector()));

	popup.addMenu(studioProgramProjector);

	QAction *studioProgramWindow =
		popup.addAction(QTStr("StudioProgramWindow"), this,
				SLOT(OpenStudioProgramWindow()));

	popup.addAction(studioProgramWindow);

	popup.exec(QCursor::pos());
}

void OBSBasic::PreviewDisabledMenu(const QPoint &pos)
{
	QMenu popup(this);
	delete previewProjectorMain;

	QAction *action =
		popup.addAction(QTStr("Basic.Main.PreviewConextMenu.Enable"),
				this, SLOT(TogglePreview()));
	action->setCheckable(true);
	action->setChecked(obs_display_enabled(ui->preview->GetDisplay()));

	previewProjectorMain = new QMenu(QTStr("PreviewProjector"));
	AddProjectorMenuMonitors(previewProjectorMain, this,
				 SLOT(OpenPreviewProjector()));

	QAction *previewWindow = popup.addAction(QTStr("PreviewWindow"), this,
						 SLOT(OpenPreviewWindow()));

	popup.addMenu(previewProjectorMain);
	popup.addAction(previewWindow);
	popup.exec(QCursor::pos());

	UNUSED_PARAMETER(pos);
}

void OBSBasic::on_actionAlwaysOnTop_triggered()
{
#ifndef _WIN32
	/* Make sure all dialogs are safely and successfully closed before
	 * switching the always on top mode due to the fact that windows all
	 * have to be recreated, so queue the actual toggle to happen after
	 * all events related to closing the dialogs have finished */
	CloseDialogs();
#endif

	QMetaObject::invokeMethod(this, "ToggleAlwaysOnTop",
				  Qt::QueuedConnection);
}

void OBSBasic::ToggleAlwaysOnTop()
{
	bool isAlwaysOnTop = IsAlwaysOnTop(this);

	//ui->actionAlwaysOnTop->setChecked(!isAlwaysOnTop);
	SetAlwaysOnTop(this, !isAlwaysOnTop);

	show();
}

void OBSBasic::GetFPSCommon(uint32_t &num, uint32_t &den) const
{
	const char *val = config_get_string(basicConfig, "Video", "FPSCommon");

	if (strcmp(val, "10") == 0) {
		num = 10;
		den = 1;
	} else if (strcmp(val, "20") == 0) {
		num = 20;
		den = 1;
	} else if (strcmp(val, "24 NTSC") == 0) {
		num = 24000;
		den = 1001;
	} else if (strcmp(val, "25 PAL") == 0) {
		num = 25;
		den = 1;
	} else if (strcmp(val, "29.97") == 0) {
		num = 30000;
		den = 1001;
	} else if (strcmp(val, "48") == 0) {
		num = 48;
		den = 1;
	} else if (strcmp(val, "50 PAL") == 0) {
		num = 50;
		den = 1;
	} else if (strcmp(val, "59.94") == 0) {
		num = 60000;
		den = 1001;
	} else if (strcmp(val, "60") == 0) {
		num = 60;
		den = 1;
	} else {
		num = 30;
		den = 1;
	}
}

void OBSBasic::GetFPSInteger(uint32_t &num, uint32_t &den) const
{
	num = (uint32_t)config_get_uint(basicConfig, "Video", "FPSInt");
	den = 1;
}

void OBSBasic::GetFPSFraction(uint32_t &num, uint32_t &den) const
{
	num = (uint32_t)config_get_uint(basicConfig, "Video", "FPSNum");
	den = (uint32_t)config_get_uint(basicConfig, "Video", "FPSDen");
}

void OBSBasic::GetFPSNanoseconds(uint32_t &num, uint32_t &den) const
{
	num = 1000000000;
	den = (uint32_t)config_get_uint(basicConfig, "Video", "FPSNS");
}

void OBSBasic::GetConfigFPS(uint32_t &num, uint32_t &den) const
{
	uint32_t type = config_get_uint(basicConfig, "Video", "FPSType");

	if (type == 1) //"Integer"
		GetFPSInteger(num, den);
	else if (type == 2) //"Fraction"
		GetFPSFraction(num, den);
	else if (false) //"Nanoseconds", currently not implemented
		GetFPSNanoseconds(num, den);
	else
		GetFPSCommon(num, den);
}

config_t *OBSBasic::Config() const
{
	return basicConfig;
}

void OBSBasic::on_actionEditTransform_triggered()
{
	if (transformWindow)
		transformWindow->close();

	transformWindow = new OBSBasicTransform(this);
	transformWindow->show();
	transformWindow->setAttribute(Qt::WA_DeleteOnClose, true);
}

static obs_transform_info copiedTransformInfo;
static obs_sceneitem_crop copiedCropInfo;

void OBSBasic::on_actionCopyTransform_triggered()
{
	auto func = [](obs_scene_t *scene, obs_sceneitem_t *item, void *param) {
		if (!obs_sceneitem_selected(item))
			return true;

		obs_sceneitem_defer_update_begin(item);
		obs_sceneitem_get_info(item, &copiedTransformInfo);
		obs_sceneitem_get_crop(item, &copiedCropInfo);
		obs_sceneitem_defer_update_end(item);

		UNUSED_PARAMETER(scene);
		UNUSED_PARAMETER(param);
		return true;
	};

	obs_scene_enum_items(GetCurrentScene(), func, nullptr);
	//ui->actionPasteTransform->setEnabled(true);
}

void OBSBasic::on_actionPasteTransform_triggered()
{
	auto func = [](obs_scene_t *scene, obs_sceneitem_t *item, void *param) {
		if (!obs_sceneitem_selected(item))
			return true;

		obs_sceneitem_defer_update_begin(item);
		obs_sceneitem_set_info(item, &copiedTransformInfo);
		obs_sceneitem_set_crop(item, &copiedCropInfo);
		obs_sceneitem_defer_update_end(item);

		UNUSED_PARAMETER(scene);
		UNUSED_PARAMETER(param);
		return true;
	};

	obs_scene_enum_items(GetCurrentScene(), func, nullptr);
}

static bool reset_tr(obs_scene_t *scene, obs_sceneitem_t *item, void *param)
{
	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, reset_tr, nullptr);
	if (!obs_sceneitem_selected(item))
		return true;
	if (obs_sceneitem_locked(item))
		return true;

	obs_sceneitem_defer_update_begin(item);

	obs_transform_info info;
	vec2_set(&info.pos, 0.0f, 0.0f);
	vec2_set(&info.scale, 1.0f, 1.0f);
	info.rot = 0.0f;
	info.alignment = OBS_ALIGN_TOP | OBS_ALIGN_LEFT;
	info.bounds_type = OBS_BOUNDS_NONE;
	info.bounds_alignment = OBS_ALIGN_CENTER;
	vec2_set(&info.bounds, 0.0f, 0.0f);
	obs_sceneitem_set_info(item, &info);

	obs_sceneitem_crop crop = {};
	obs_sceneitem_set_crop(item, &crop);

	obs_sceneitem_defer_update_end(item);

	UNUSED_PARAMETER(scene);
	UNUSED_PARAMETER(param);
	return true;
}

void OBSBasic::on_actionResetTransform_triggered()
{
	obs_scene_enum_items(GetCurrentScene(), reset_tr, nullptr);
}

static void GetItemBox(obs_sceneitem_t *item, vec3 &tl, vec3 &br)
{
	matrix4 boxTransform;
	obs_sceneitem_get_box_transform(item, &boxTransform);

	vec3_set(&tl, M_INFINITE, M_INFINITE, 0.0f);
	vec3_set(&br, -M_INFINITE, -M_INFINITE, 0.0f);

	auto GetMinPos = [&](float x, float y) {
		vec3 pos;
		vec3_set(&pos, x, y, 0.0f);
		vec3_transform(&pos, &pos, &boxTransform);
		vec3_min(&tl, &tl, &pos);
		vec3_max(&br, &br, &pos);
	};

	GetMinPos(0.0f, 0.0f);
	GetMinPos(1.0f, 0.0f);
	GetMinPos(0.0f, 1.0f);
	GetMinPos(1.0f, 1.0f);
}

static vec3 GetItemTL(obs_sceneitem_t *item)
{
	vec3 tl, br;
	GetItemBox(item, tl, br);
	return tl;
}

static void SetItemTL(obs_sceneitem_t *item, const vec3 &tl)
{
	vec3 newTL;
	vec2 pos;

	obs_sceneitem_get_pos(item, &pos);
	newTL = GetItemTL(item);
	pos.x += tl.x - newTL.x;
	pos.y += tl.y - newTL.y;
	obs_sceneitem_set_pos(item, &pos);
}

static bool RotateSelectedSources(obs_scene_t *scene, obs_sceneitem_t *item,
				  void *param)
{
	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, RotateSelectedSources,
					       param);
	if (!obs_sceneitem_selected(item))
		return true;
	if (obs_sceneitem_locked(item))
		return true;

	float rot = *reinterpret_cast<float *>(param);

	vec3 tl = GetItemTL(item);

	rot += obs_sceneitem_get_rot(item);
	if (rot >= 360.0f)
		rot -= 360.0f;
	else if (rot <= -360.0f)
		rot += 360.0f;
	obs_sceneitem_set_rot(item, rot);

	obs_sceneitem_force_update_transform(item);

	SetItemTL(item, tl);

	UNUSED_PARAMETER(scene);
	return true;
};

void OBSBasic::on_actionRotate90CW_triggered()
{
	float f90CW = 90.0f;
	obs_scene_enum_items(GetCurrentScene(), RotateSelectedSources, &f90CW);
}

void OBSBasic::on_actionRotate90CCW_triggered()
{
	float f90CCW = -90.0f;
	obs_scene_enum_items(GetCurrentScene(), RotateSelectedSources, &f90CCW);
}

void OBSBasic::on_actionRotate180_triggered()
{
	float f180 = 180.0f;
	obs_scene_enum_items(GetCurrentScene(), RotateSelectedSources, &f180);
}

static bool MultiplySelectedItemScale(obs_scene_t *scene, obs_sceneitem_t *item,
				      void *param)
{
	vec2 &mul = *reinterpret_cast<vec2 *>(param);

	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, MultiplySelectedItemScale,
					       param);
	if (!obs_sceneitem_selected(item))
		return true;
	if (obs_sceneitem_locked(item))
		return true;

	vec3 tl = GetItemTL(item);

	vec2 scale;
	obs_sceneitem_get_scale(item, &scale);
	vec2_mul(&scale, &scale, &mul);
	obs_sceneitem_set_scale(item, &scale);

	obs_sceneitem_force_update_transform(item);

	SetItemTL(item, tl);

	UNUSED_PARAMETER(scene);
	return true;
}

void OBSBasic::on_actionFlipHorizontal_triggered()
{
	vec2 scale;
	vec2_set(&scale, -1.0f, 1.0f);
	obs_scene_enum_items(GetCurrentScene(), MultiplySelectedItemScale,
			     &scale);
}

void OBSBasic::on_actionFlipVertical_triggered()
{
	vec2 scale;
	vec2_set(&scale, 1.0f, -1.0f);
	obs_scene_enum_items(GetCurrentScene(), MultiplySelectedItemScale,
			     &scale);
}

static bool CenterAlignSelectedItems(obs_scene_t *scene, obs_sceneitem_t *item,
				     void *param)
{
	obs_bounds_type boundsType =
		*reinterpret_cast<obs_bounds_type *>(param);

	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, CenterAlignSelectedItems,
					       param);
	if (!obs_sceneitem_selected(item))
		return true;
	if (obs_sceneitem_locked(item))
		return true;

	obs_video_info ovi;
	obs_get_video_info(&ovi);

	obs_transform_info itemInfo;
	vec2_set(&itemInfo.pos, 0.0f, 0.0f);
	vec2_set(&itemInfo.scale, 1.0f, 1.0f);
	itemInfo.alignment = OBS_ALIGN_LEFT | OBS_ALIGN_TOP;
	itemInfo.rot = 0.0f;

	vec2_set(&itemInfo.bounds, float(ovi.base_width),
		 float(ovi.base_height));
	itemInfo.bounds_type = boundsType;
	itemInfo.bounds_alignment = OBS_ALIGN_CENTER;

	obs_sceneitem_set_info(item, &itemInfo);

	UNUSED_PARAMETER(scene);
	return true;
}

void OBSBasic::on_actionFitToScreen_triggered()
{
	obs_bounds_type boundsType = OBS_BOUNDS_SCALE_INNER;
	obs_scene_enum_items(GetCurrentScene(), CenterAlignSelectedItems,
			     &boundsType);
}

void OBSBasic::on_actionStretchToScreen_triggered()
{
	obs_bounds_type boundsType = OBS_BOUNDS_STRETCH;
	obs_scene_enum_items(GetCurrentScene(), CenterAlignSelectedItems,
			     &boundsType);
}

enum class CenterType {
	Scene,
	Vertical,
	Horizontal,
};

static bool center_to_scene(obs_scene_t *, obs_sceneitem_t *item, void *param)
{
	CenterType centerType = *reinterpret_cast<CenterType *>(param);

	vec3 tl, br, itemCenter, screenCenter, offset;
	obs_video_info ovi;
	obs_transform_info oti;

	if (obs_sceneitem_is_group(item))
		obs_sceneitem_group_enum_items(item, center_to_scene,
					       &centerType);
	if (!obs_sceneitem_selected(item))
		return true;
	if (obs_sceneitem_locked(item))
		return true;

	obs_get_video_info(&ovi);
	obs_sceneitem_get_info(item, &oti);

	if (centerType == CenterType::Scene)
		vec3_set(&screenCenter, float(ovi.base_width),
			 float(ovi.base_height), 0.0f);
	else if (centerType == CenterType::Vertical)
		vec3_set(&screenCenter, float(oti.bounds.x),
			 float(ovi.base_height), 0.0f);
	else if (centerType == CenterType::Horizontal)
		vec3_set(&screenCenter, float(ovi.base_width),
			 float(oti.bounds.y), 0.0f);

	vec3_mulf(&screenCenter, &screenCenter, 0.5f);

	GetItemBox(item, tl, br);

	vec3_sub(&itemCenter, &br, &tl);
	vec3_mulf(&itemCenter, &itemCenter, 0.5f);
	vec3_add(&itemCenter, &itemCenter, &tl);

	vec3_sub(&offset, &screenCenter, &itemCenter);
	vec3_add(&tl, &tl, &offset);

	if (centerType == CenterType::Vertical)
		tl.x = oti.pos.x;
	else if (centerType == CenterType::Horizontal)
		tl.y = oti.pos.y;

	SetItemTL(item, tl);
	return true;
};

void OBSBasic::on_actionCenterToScreen_triggered()
{
	CenterType centerType = CenterType::Scene;
	obs_scene_enum_items(GetCurrentScene(), center_to_scene, &centerType);
}

void OBSBasic::on_actionVerticalCenter_triggered()
{
	CenterType centerType = CenterType::Vertical;
	obs_scene_enum_items(GetCurrentScene(), center_to_scene, &centerType);
}

void OBSBasic::on_actionHorizontalCenter_triggered()
{
	CenterType centerType = CenterType::Horizontal;
	obs_scene_enum_items(GetCurrentScene(), center_to_scene, &centerType);
}

void OBSBasic::EnablePreviewDisplay(bool enable)
{
	obs_display_set_enabled(ui->preview->GetDisplay(), enable);
	ui->preview->setVisible(enable);
	ui->previewDisabledWidget->setVisible(!enable);
}

void OBSBasic::TogglePreview()
{
	previewEnabled = !previewEnabled;
	EnablePreviewDisplay(previewEnabled);
}

void OBSBasic::EnablePreview()
{
	if (previewProgramMode)
		return;

	previewEnabled = true;
	EnablePreviewDisplay(true);
}

void OBSBasic::DisablePreview()
{
	if (previewProgramMode)
		return;

	previewEnabled = false;
	EnablePreviewDisplay(false);
}

static bool nudge_callback(obs_scene_t *, obs_sceneitem_t *item, void *param)
{
	if (obs_sceneitem_locked(item))
		return true;

	struct vec2 &offset = *reinterpret_cast<struct vec2 *>(param);
	struct vec2 pos;

	if (!obs_sceneitem_selected(item)) {
		if (obs_sceneitem_is_group(item)) {
			struct vec3 offset3;
			vec3_set(&offset3, offset.x, offset.y, 0.0f);

			struct matrix4 matrix;
			obs_sceneitem_get_draw_transform(item, &matrix);
			vec4_set(&matrix.t, 0.0f, 0.0f, 0.0f, 1.0f);
			matrix4_inv(&matrix, &matrix);
			vec3_transform(&offset3, &offset3, &matrix);

			struct vec2 new_offset;
			vec2_set(&new_offset, offset3.x, offset3.y);
			obs_sceneitem_group_enum_items(item, nudge_callback,
						       &new_offset);
		}

		return true;
	}

	obs_sceneitem_get_pos(item, &pos);
	vec2_add(&pos, &pos, &offset);
	obs_sceneitem_set_pos(item, &pos);
	return true;
}

void OBSBasic::Nudge(int dist, MoveDir dir)
{
	if (ui->preview->Locked())
		return;

	struct vec2 offset;
	vec2_set(&offset, 0.0f, 0.0f);

	switch (dir) {
	case MoveDir::Up:
		offset.y = (float)-dist;
		break;
	case MoveDir::Down:
		offset.y = (float)dist;
		break;
	case MoveDir::Left:
		offset.x = (float)-dist;
		break;
	case MoveDir::Right:
		offset.x = (float)dist;
		break;
	}

	obs_scene_enum_items(GetCurrentScene(), nudge_callback, &offset);
}

void OBSBasic::NudgeUp()
{
	Nudge(1, MoveDir::Up);
}
void OBSBasic::NudgeDown()
{
	Nudge(1, MoveDir::Down);
}
void OBSBasic::NudgeLeft()
{
	Nudge(1, MoveDir::Left);
}
void OBSBasic::NudgeRight()
{
	Nudge(1, MoveDir::Right);
}

void OBSBasic::DeleteProjector(OBSProjector *projector)
{
	for (size_t i = 0; i < projectors.size(); i++) {
		if (projectors[i] == projector) {
			projectors[i]->deleteLater();
			projectors.erase(projectors.begin() + i);
			break;
		}
	}
}

OBSProjector *OBSBasic::OpenProjector(obs_source_t *source, int monitor,
				      ProjectorType type)
{
	/* seriously?  10 monitors? */
	if (monitor > 9 || monitor > QGuiApplication::screens().size() - 1)
		return nullptr;

	OBSProjector *projector =
		new OBSProjector(nullptr, source, monitor, type);

	if (projector)
		projectors.emplace_back(projector);

	return projector;
}

void OBSBasic::OpenStudioProgramProjector()
{
	int monitor = sender()->property("monitor").toInt();
	OpenProjector(nullptr, monitor, ProjectorType::StudioProgram);
}

void OBSBasic::OpenPreviewProjector()
{
	int monitor = sender()->property("monitor").toInt();
	OpenProjector(nullptr, monitor, ProjectorType::Preview);
}

void OBSBasic::OpenSourceProjector()
{
	int monitor = sender()->property("monitor").toInt();
	OBSSceneItem item = GetCurrentSceneItem();
	if (!item)
		return;

	OpenProjector(obs_sceneitem_get_source(item), monitor,
		      ProjectorType::Source);
}

void OBSBasic::OpenMultiviewProjector()
{
	int monitor = sender()->property("monitor").toInt();
	OpenProjector(nullptr, monitor, ProjectorType::Multiview);
}

void OBSBasic::OpenSceneProjector()
{
	int monitor = sender()->property("monitor").toInt();
	OBSScene scene = GetCurrentScene();
	if (!scene)
		return;

	OpenProjector(obs_scene_get_source(scene), monitor,
		      ProjectorType::Scene);
}

void OBSBasic::OpenStudioProgramWindow()
{
	OpenProjector(nullptr, -1, ProjectorType::StudioProgram);
}

void OBSBasic::OpenPreviewWindow()
{
	OpenProjector(nullptr, -1, ProjectorType::Preview);
}

void OBSBasic::OpenSourceWindow()
{
	OBSSceneItem item = GetCurrentSceneItem();
	if (!item)
		return;

	OBSSource source = obs_sceneitem_get_source(item);

	OpenProjector(obs_sceneitem_get_source(item), -1,
		      ProjectorType::Source);
}

void OBSBasic::OpenMultiviewWindow()
{
	OpenProjector(nullptr, -1, ProjectorType::Multiview);
}

void OBSBasic::OpenSceneWindow()
{
	OBSScene scene = GetCurrentScene();
	if (!scene)
		return;

	OBSSource source = obs_scene_get_source(scene);

	OpenProjector(obs_scene_get_source(scene), -1, ProjectorType::Scene);
}

void OBSBasic::OpenSavedProjectors()
{
	for (SavedProjectorInfo *info : savedProjectorsArray) {
		OpenSavedProjector(info);
	}
}

void OBSBasic::OpenSavedProjector(SavedProjectorInfo *info)
{
	if (info) {
		OBSProjector *projector = nullptr;
		switch (info->type) {
		case ProjectorType::Source:
		case ProjectorType::Scene: {
			OBSSource source =
				obs_get_source_by_name(info->name.c_str());
			if (!source)
				return;

			projector = OpenProjector(source, info->monitor,
						  info->type);

			obs_source_release(source);
			break;
		}
		default: {
			projector = OpenProjector(nullptr, info->monitor,
						  info->type);
			break;
		}
		}

		if (projector && !info->geometry.empty() && info->monitor < 0) {
			QByteArray byteArray = QByteArray::fromBase64(
				QByteArray(info->geometry.c_str()));
			projector->restoreGeometry(byteArray);

			if (!WindowPositionValid(projector->normalGeometry())) {
				QRect rect = App()->desktop()->geometry();
				projector->setGeometry(QStyle::alignedRect(
					Qt::LeftToRight, Qt::AlignCenter,
					size(), rect));
			}
		}
	}
}

void OBSBasic::on_actionFullscreenInterface_triggered()
{
	if (!isFullScreen())
		showFullScreen();
	else
		showNormal();
}

void OBSBasic::UpdateTitleBar()
{
	stringstream name;

	const char *profile =
		config_get_string(App()->GlobalConfig(), "Basic", "Profile");
	const char *sceneCollection = config_get_string(
		App()->GlobalConfig(), "Basic", "SceneCollection");

	name << "OBS ";
	if (previewProgramMode)
		name << "Studio ";

	name << App()->GetVersionString();
	if (App()->IsPortableMode())
		name << " - Portable Mode";

	name << " - " << Str("TitleBar.Profile") << ": " << profile;
	name << " - " << Str("TitleBar.Scenes") << ": " << sceneCollection;

	setWindowTitle(QT_UTF8(name.str().c_str()));
}

int OBSBasic::GetProfilePath(char *path, size_t size, const char *file) const
{
	char profiles_path[512];
	const char *profile =
		config_get_string(App()->GlobalConfig(), "Basic", "ProfileDir");
	int ret;

	if (!profile)
		return -1;
	if (!path)
		return -1;
	if (!file)
		file = "";

	ret = GetConfigPath(profiles_path, 512, "obs-studio/basic/profiles");
	if (ret <= 0)
		return ret;

	if (!*file)
		return snprintf(path, size, "%s/%s", profiles_path, profile);

	return snprintf(path, size, "%s/%s/%s", profiles_path, profile, file);
}

void OBSBasic::on_resetUI_triggered()
{
	/* prune deleted extra docks */
	for (int i = extraDocks.size() - 1; i >= 0; i--) {
		if (!extraDocks[i]) {
			extraDocks.removeAt(i);
		}
	}

	if (extraDocks.size()) {
		QMessageBox::StandardButton button = QMessageBox::question(
			this, QTStr("ResetUIWarning.Title"),
			QTStr("ResetUIWarning.Text"));

		if (button == QMessageBox::No)
			return;
	}

	/* undock/hide/center extra docks */
	for (int i = extraDocks.size() - 1; i >= 0; i--) {
		if (extraDocks[i]) {
			extraDocks[i]->setVisible(true);
			extraDocks[i]->setFloating(true);
			extraDocks[i]->move(frameGeometry().topLeft() +
					    rect().center() -
					    extraDocks[i]->rect().center());
			extraDocks[i]->setVisible(false);
		}
	}

	// restoreState(startingDockLayout);

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
	int cx = width();
	int cy = height();

	int cx22_5 = cx * 225 / 1000;
	int cx5 = cx * 5 / 100;

	cy = cy * 225 / 1000;

	int mixerSize = cx - (cx22_5 * 2 + cx5 * 2);

	QList<QDockWidget *> docks{/*ui->scenesDock, ui->sourcesDock,
				   ui->mixerDock, ui->transitionsDock,
				   ui->controlsDock*/};

	QList<int> sizes{cx22_5, cx22_5, mixerSize, cx5, cx5};

	//ui->scenesDock->setVisible(true);
	//ui->sourcesDock->setVisible(true);
	//ui->mixerDock->setVisible(true);
	// ui->transitionsDock->setVisible(true);
	//ui->controlsDock->setVisible(true);
	//statsDock->setVisible(false);
	//statsDock->setFloating(true);

	// resizeDocks(docks, {cy, cy, cy, cy, cy}, Qt::Vertical);
	// resizeDocks(docks, sizes, Qt::Horizontal);
#endif
}

void OBSBasic::on_lockUI_toggled(bool lock)
{
	QDockWidget::DockWidgetFeatures features =
		lock ? QDockWidget::NoDockWidgetFeatures
		     : QDockWidget::AllDockWidgetFeatures;

	QDockWidget::DockWidgetFeatures mainFeatures = features;
	mainFeatures &= ~QDockWidget::QDockWidget::DockWidgetClosable;

	//ui->scenesDock->setFeatures(mainFeatures);
	//ui->sourcesDock->setFeatures(mainFeatures);
	//ui->mixerDock->setFeatures(mainFeatures);
	// ui->transitionsDock->setFeatures(mainFeatures);
	//ui->controlsDock->setFeatures(mainFeatures);
	//statsDock->setFeatures(features);

	for (int i = extraDocks.size() - 1; i >= 0; i--) {
		if (!extraDocks[i]) {
			extraDocks.removeAt(i);
		} else {
			extraDocks[i]->setFeatures(features);
		}
	}
}

void OBSBasic::on_toggleListboxToolbars_toggled(bool visible)
{
	//ui->sourcesToolbar->setVisible(visible);
	//ui->scenesToolbar->setVisible(visible);

	config_set_bool(App()->GlobalConfig(), "BasicWindow",
			"ShowListboxToolbars", visible);
}

void OBSBasic::on_toggleStatusBar_toggled(bool visible)
{
	// ui->statusbar->setVisible(visible);

	config_set_bool(App()->GlobalConfig(), "BasicWindow", "ShowStatusBar",
			visible);
}

void OBSBasic::on_toggleSourceIcons_toggled(bool visible)
{
	ui->sources->SetIconsVisible(visible);
	if (advAudioWindow != nullptr)
		advAudioWindow->SetIconsVisible(visible);

	config_set_bool(App()->GlobalConfig(), "BasicWindow", "ShowSourceIcons",
			visible);
}

void OBSBasic::on_actionLockPreview_triggered()
{
	ui->preview->ToggleLocked();
	//ui->actionLockPreview->setChecked(ui->preview->Locked());
}

void OBSBasic::on_scalingMenu_aboutToShow()
{
	obs_video_info ovi;
	obs_get_video_info(&ovi);
#if 0
	QAction *action = ui->actionScaleCanvas;
	QString text = QTStr("Basic.MainMenu.Edit.Scale.Canvas");
	text = text.arg(QString::number(ovi.base_width),
			QString::number(ovi.base_height));
	action->setText(text);

	action = ui->actionScaleOutput;
	text = QTStr("Basic.MainMenu.Edit.Scale.Output");
	text = text.arg(QString::number(ovi.output_width),
			QString::number(ovi.output_height));
	action->setText(text);
	action->setVisible(!(ovi.output_width == ovi.base_width &&
			     ovi.output_height == ovi.base_height));

	UpdatePreviewScalingMenu();
#endif
}

void OBSBasic::on_actionScaleWindow_triggered()
{
	ui->preview->SetFixedScaling(false);
	ui->preview->ResetScrollingOffset();
	emit ui->preview->DisplayResized();
}

void OBSBasic::on_actionScaleCanvas_triggered()
{
	ui->preview->SetFixedScaling(true);
	ui->preview->SetScalingLevel(0);
	emit ui->preview->DisplayResized();
}

void OBSBasic::on_actionScaleOutput_triggered()
{
	obs_video_info ovi;
	obs_get_video_info(&ovi);

	ui->preview->SetFixedScaling(true);
	float scalingAmount = float(ovi.output_width) / float(ovi.base_width);
	// log base ZOOM_SENSITIVITY of x = log(x) / log(ZOOM_SENSITIVITY)
	int32_t approxScalingLevel =
		int32_t(round(log(scalingAmount) / log(ZOOM_SENSITIVITY)));
	ui->preview->SetScalingLevel(approxScalingLevel);
	ui->preview->SetScalingAmount(scalingAmount);
	emit ui->preview->DisplayResized();
}

void OBSBasic::SetShowing(bool showing)
{
	if (!showing && isVisible()) {
		config_set_string(App()->GlobalConfig(), "BasicWindow",
				  "geometry",
				  saveGeometry().toBase64().constData());

		/* hide all visible child dialogs */
		visDlgPositions.clear();
		if (!visDialogs.isEmpty()) {
			for (QDialog *dlg : visDialogs) {
				visDlgPositions.append(dlg->pos());
				dlg->hide();
			}
		}

		if (showHide)
			showHide->setText(QTStr("Basic.SystemTray.Show"));
		QTimer::singleShot(250, this, SLOT(hide()));

		setVisible(false);

#ifdef __APPLE__
		EnableOSXDockIcon(false);
#endif

	} else if (showing && !isVisible()) {
		if (showHide)
			showHide->setText(QTStr("Basic.SystemTray.Hide"));
		QTimer::singleShot(250, this, SLOT(show()));

		setVisible(true);

#ifdef __APPLE__
		EnableOSXDockIcon(true);
#endif

		/* raise and activate window to ensure it is on top */
		raise();
		activateWindow();

		/* show all child dialogs that was visible earlier */
		if (!visDialogs.isEmpty()) {
			for (int i = 0; i < visDialogs.size(); ++i) {
				QDialog *dlg = visDialogs[i];
				dlg->move(visDlgPositions[i]);
				dlg->show();
			}
		}

		/* Unminimize window if it was hidden to tray instead of task
		 * bar. */
		if (sysTrayMinimizeToTray()) {
			Qt::WindowStates state;
			state = windowState() & ~Qt::WindowMinimized;
			state |= Qt::WindowActive;
			setWindowState(state);
		}
	}
}

void OBSBasic::ToggleShowHide()
{
	bool showing = isVisible();
	if (showing) {
		/* check for modal dialogs */
		EnumDialogs();
		if (!modalDialogs.isEmpty() || !visMsgBoxes.isEmpty())
			return;
	}
	SetShowing(!showing);
}

void OBSBasic::SystemTrayInit()
{
	trayIcon.reset(new QSystemTrayIcon(
		QIcon::fromTheme("obs-tray", QIcon(":/res/images/obs.png")),
		this));
	trayIcon->setToolTip("OBS Studio");

	showHide = new QAction(QTStr("Basic.SystemTray.Show"), trayIcon.data());
	//sysTrayStream = new QAction(QTStr("Basic.Main.StartStreaming"),
//				    trayIcon.data());
	sysTrayRecord = new QAction(QTStr("Basic.Main.StartRecording"),
				    trayIcon.data());
	sysTrayReplayBuffer = new QAction(QTStr("Basic.Main.StartReplayBuffer"),
					  trayIcon.data());
	exit = new QAction(QTStr("Exit"), trayIcon.data());

	trayMenu = new QMenu;
	previewProjector = new QMenu(QTStr("PreviewProjector"));
	studioProgramProjector = new QMenu(QTStr("StudioProgramProjector"));
	AddProjectorMenuMonitors(previewProjector, this,
				 SLOT(OpenPreviewProjector()));
	AddProjectorMenuMonitors(studioProgramProjector, this,
				 SLOT(OpenStudioProgramProjector()));
	trayMenu->addAction(showHide);
	trayMenu->addMenu(previewProjector);
	trayMenu->addMenu(studioProgramProjector);
	// trayMenu->addAction(sysTrayStream);
	trayMenu->addAction(sysTrayRecord);
	trayMenu->addAction(sysTrayReplayBuffer);
	trayMenu->addAction(exit);
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	if (outputHandler && !outputHandler->replayBuffer)
		sysTrayReplayBuffer->setEnabled(false);

	connect(trayIcon.data(),
		SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
		SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	connect(showHide, SIGNAL(triggered()), this, SLOT(ToggleShowHide()));
	//connect(sysTrayStream, SIGNAL(triggered()), this,
//		SLOT(on_streamButton_clicked()));
	connect(sysTrayRecord, SIGNAL(triggered()), this,
		SLOT(on_recordButton_clicked()));
	connect(sysTrayReplayBuffer.data(), &QAction::triggered, this,
		&OBSBasic::ReplayBufferClicked);
	connect(exit, SIGNAL(triggered()), this, SLOT(close()));
}

void OBSBasic::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	// Refresh projector list
	previewProjector->clear();
	studioProgramProjector->clear();
	AddProjectorMenuMonitors(previewProjector, this,
				 SLOT(OpenPreviewProjector()));
	AddProjectorMenuMonitors(studioProgramProjector, this,
				 SLOT(OpenStudioProgramProjector()));

	if (reason == QSystemTrayIcon::Trigger) {
		EnablePreviewDisplay(previewEnabled && !isVisible());
		ToggleShowHide();
	}
}

void OBSBasic::SysTrayNotify(const QString &text,
			     QSystemTrayIcon::MessageIcon n)
{
	if (trayIcon && QSystemTrayIcon::supportsMessages()) {
		QSystemTrayIcon::MessageIcon icon =
			QSystemTrayIcon::MessageIcon(n);
		trayIcon->showMessage("OBS Studio", text, icon, 10000);
	}
}

void OBSBasic::SystemTray(bool firstStarted)
{
	if (!QSystemTrayIcon::isSystemTrayAvailable())
		return;
	if (!trayIcon && !firstStarted)
		return;

	bool sysTrayWhenStarted = config_get_bool(
		GetGlobalConfig(), "BasicWindow", "SysTrayWhenStarted");
	bool sysTrayEnabled = config_get_bool(GetGlobalConfig(), "BasicWindow",
					      "SysTrayEnabled");

	if (firstStarted)
		SystemTrayInit();

	if (!sysTrayWhenStarted && !sysTrayEnabled) {
		trayIcon->hide();
	} else if ((sysTrayWhenStarted && sysTrayEnabled) ||
		   opt_minimize_tray) {
		trayIcon->show();
		if (firstStarted) {
			QTimer::singleShot(50, this, SLOT(hide()));
			EnablePreviewDisplay(false);
			setVisible(false);
#ifdef __APPLE__
			EnableOSXDockIcon(false);
#endif
			opt_minimize_tray = false;
		}
	} else if (sysTrayEnabled) {
		trayIcon->show();
	} else if (!sysTrayEnabled) {
		trayIcon->hide();
	} else if (!sysTrayWhenStarted && sysTrayEnabled) {
		trayIcon->hide();
	}

	if (isVisible())
		showHide->setText(QTStr("Basic.SystemTray.Hide"));
	else
		showHide->setText(QTStr("Basic.SystemTray.Show"));
}

bool OBSBasic::sysTrayMinimizeToTray()
{
	return config_get_bool(GetGlobalConfig(), "BasicWindow",
			       "SysTrayMinimizeToTray");
}

void OBSBasic::on_actionCopySource_triggered()
{
	copyStrings.clear();
	bool allowPastingDuplicate = true;

	for (auto &selectedSource : GetAllSelectedSourceItems()) {
		OBSSceneItem item = ui->sources->Get(selectedSource.row());
		if (!item)
			continue;

		on_actionCopyTransform_triggered();

		OBSSource source = obs_sceneitem_get_source(item);

		copyStrings.push_front(obs_source_get_name(source));

		copyVisible = obs_sceneitem_visible(item);

		uint32_t output_flags = obs_source_get_output_flags(source);
		if (!(output_flags & OBS_SOURCE_DO_NOT_DUPLICATE) == 0)
			allowPastingDuplicate = false;
	}

	//ui->actionPasteRef->setEnabled(true);
	//ui->actionPasteDup->setEnabled(allowPastingDuplicate);
}

void OBSBasic::on_actionPasteRef_triggered()
{
	for (auto &copyString : copyStrings) {
		/* do not allow duplicate refs of the same group in the same scene */
		OBSScene scene = GetCurrentScene();
		if (!!obs_scene_get_group(scene, copyString))
			continue;

		OBSBasicSourceSelect::SourcePaste(copyString, copyVisible,
						  false);
		on_actionPasteTransform_triggered();
	}
}

void OBSBasic::on_actionPasteDup_triggered()
{
	for (auto &copyString : copyStrings) {
		OBSBasicSourceSelect::SourcePaste(copyString, copyVisible,
						  true);
		on_actionPasteTransform_triggered();
	}
}

void OBSBasic::AudioMixerCopyFilters()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *source = vol->GetSource();

	copyFiltersString = obs_source_get_name(source);
}

void OBSBasic::AudioMixerPasteFilters()
{
	QAction *action = reinterpret_cast<QAction *>(sender());
	VolControl *vol = action->property("volControl").value<VolControl *>();
	obs_source_t *dstSource = vol->GetSource();

	OBSSource source = obs_get_source_by_name(copyFiltersString);
	obs_source_release(source);

	if (source == dstSource)
		return;

	obs_source_copy_filters(dstSource, source);
}

void OBSBasic::SceneCopyFilters()
{
	copyFiltersString = obs_source_get_name(GetCurrentSceneSource());
}

void OBSBasic::ScenePasteFilters()
{
	OBSSource source = obs_get_source_by_name(copyFiltersString);
	obs_source_release(source);

	OBSSource dstSource = GetCurrentSceneSource();

	if (source == dstSource)
		return;

	obs_source_copy_filters(dstSource, source);
}

void OBSBasic::on_actionCopyFilters_triggered()
{
	OBSSceneItem item = GetCurrentSceneItem();

	if (!item)
		return;

	OBSSource source = obs_sceneitem_get_source(item);

	copyFiltersString = obs_source_get_name(source);

	//ui->actionPasteFilters->setEnabled(true);
}

void OBSBasic::on_actionPasteFilters_triggered()
{
	OBSSource source = obs_get_source_by_name(copyFiltersString);
	obs_source_release(source);

	OBSSceneItem sceneItem = GetCurrentSceneItem();
	OBSSource dstSource = obs_sceneitem_get_source(sceneItem);

	if (source == dstSource)
		return;

	obs_source_copy_filters(dstSource, source);
}

static void ConfirmColor(SourceTree *sources, const QColor &color,
			 QModelIndexList selectedItems)
{
	for (int x = 0; x < selectedItems.count(); x++) {
		SourceTreeItem *treeItem =
			sources->GetItemWidget(selectedItems[x].row());
		treeItem->setStyleSheet("background: " +
					color.name(QColor::HexArgb));
		treeItem->style()->unpolish(treeItem);
		treeItem->style()->polish(treeItem);

		OBSSceneItem sceneItem = sources->Get(selectedItems[x].row());
		obs_data_t *privData =
			obs_sceneitem_get_private_settings(sceneItem);
		obs_data_set_int(privData, "color-preset", 1);
		obs_data_set_string(privData, "color",
				    QT_TO_UTF8(color.name(QColor::HexArgb)));
		obs_data_release(privData);
	}
}

void OBSBasic::ColorChange()
{
	QModelIndexList selectedItems =
		ui->sources->selectionModel()->selectedIndexes();
	QAction *action = qobject_cast<QAction *>(sender());
	QPushButton *colorButton = qobject_cast<QPushButton *>(sender());

	if (selectedItems.count() == 0)
		return;

	if (colorButton) {
		int preset = colorButton->property("bgColor").value<int>();

		for (int x = 0; x < selectedItems.count(); x++) {
			SourceTreeItem *treeItem = ui->sources->GetItemWidget(
				selectedItems[x].row());
			treeItem->setStyleSheet("");
			treeItem->setProperty("bgColor", preset);
			treeItem->style()->unpolish(treeItem);
			treeItem->style()->polish(treeItem);

			OBSSceneItem sceneItem =
				ui->sources->Get(selectedItems[x].row());
			obs_data_t *privData =
				obs_sceneitem_get_private_settings(sceneItem);
			obs_data_set_int(privData, "color-preset", preset + 1);
			obs_data_set_string(privData, "color", "");
			obs_data_release(privData);
		}

		for (int i = 1; i < 9; i++) {
			stringstream button;
			button << "preset" << i;
			QPushButton *cButton =
				colorButton->parentWidget()
					->findChild<QPushButton *>(
						button.str().c_str());
			cButton->setStyleSheet("border: 1px solid black");
		}

		colorButton->setStyleSheet("border: 2px solid black");
	} else if (action) {
		int preset = action->property("bgColor").value<int>();

		if (preset == 1) {
			OBSSceneItem curSceneItem = GetCurrentSceneItem();
			SourceTreeItem *curTreeItem =
				GetItemWidgetFromSceneItem(curSceneItem);
			obs_data_t *curPrivData =
				obs_sceneitem_get_private_settings(
					curSceneItem);

			int oldPreset =
				obs_data_get_int(curPrivData, "color-preset");
			const QString oldSheet = curTreeItem->styleSheet();

			auto liveChangeColor = [=](const QColor &color) {
				if (color.isValid()) {
					curTreeItem->setStyleSheet(
						"background: " +
						color.name(QColor::HexArgb));
				}
			};

			auto changedColor = [=](const QColor &color) {
				if (color.isValid()) {
					ConfirmColor(ui->sources, color,
						     selectedItems);
				}
			};

			auto rejected = [=]() {
				if (oldPreset == 1) {
					curTreeItem->setStyleSheet(oldSheet);
					curTreeItem->setProperty("bgColor", 0);
				} else if (oldPreset == 0) {
					curTreeItem->setStyleSheet(
						"background: none");
					curTreeItem->setProperty("bgColor", 0);
				} else {
					curTreeItem->setStyleSheet("");
					curTreeItem->setProperty("bgColor",
								 oldPreset - 1);
				}

				curTreeItem->style()->unpolish(curTreeItem);
				curTreeItem->style()->polish(curTreeItem);
			};

			QColorDialog::ColorDialogOptions options =
				QColorDialog::ShowAlphaChannel;

			const char *oldColor =
				obs_data_get_string(curPrivData, "color");
			const char *customColor = *oldColor != 0 ? oldColor
								 : "#55FF0000";
#ifndef _WIN32
			options |= QColorDialog::DontUseNativeDialog;
#endif

			QColorDialog *colorDialog = new QColorDialog(this);
			colorDialog->setOptions(options);
			colorDialog->setCurrentColor(QColor(customColor));
			connect(colorDialog, &QColorDialog::currentColorChanged,
				liveChangeColor);
			connect(colorDialog, &QColorDialog::colorSelected,
				changedColor);
			connect(colorDialog, &QColorDialog::rejected, rejected);
			colorDialog->open();

			obs_data_release(curPrivData);
		} else {
			for (int x = 0; x < selectedItems.count(); x++) {
				SourceTreeItem *treeItem =
					ui->sources->GetItemWidget(
						selectedItems[x].row());
				treeItem->setStyleSheet("background: none");
				treeItem->setProperty("bgColor", preset);
				treeItem->style()->unpolish(treeItem);
				treeItem->style()->polish(treeItem);

				OBSSceneItem sceneItem = ui->sources->Get(
					selectedItems[x].row());
				obs_data_t *privData =
					obs_sceneitem_get_private_settings(
						sceneItem);
				obs_data_set_int(privData, "color-preset",
						 preset);
				obs_data_set_string(privData, "color", "");
				obs_data_release(privData);
			}
		}
	}
}

SourceTreeItem *OBSBasic::GetItemWidgetFromSceneItem(obs_sceneitem_t *sceneItem)
{
	int i = 0;
	SourceTreeItem *treeItem = ui->sources->GetItemWidget(i);
	OBSSceneItem item = ui->sources->Get(i);
	int64_t id = obs_sceneitem_get_id(sceneItem);
	while (treeItem && obs_sceneitem_get_id(item) != id) {
		i++;
		treeItem = ui->sources->GetItemWidget(i);
		item = ui->sources->Get(i);
	}
	if (treeItem)
		return treeItem;

	return nullptr;
}

void OBSBasic::on_autoConfigure_triggered()
{
	AutoConfig test(this);
	test.setModal(true);
	test.show();
	test.exec();
}

void OBSBasic::on_stats_triggered()
{
	if (!stats.isNull()) {
		stats->show();
		stats->raise();
		return;
	}

	OBSBasicStats *statsDlg;
	statsDlg = new OBSBasicStats(nullptr);
	statsDlg->show();
	stats = statsDlg;
}

void OBSBasic::on_actionShowAbout_triggered()
{
	if (about)
		about->close();

	about = new OBSAbout(this);
	about->show();

	about->setAttribute(Qt::WA_DeleteOnClose, true);
}

void OBSBasic::ResizeOutputSizeOfSource()
{
	if (obs_video_active())
		return;

	QMessageBox resize_output(this);
	resize_output.setText(QTStr("ResizeOutputSizeOfSource.Text") + "\n\n" +
			      QTStr("ResizeOutputSizeOfSource.Continue"));
	QAbstractButton *Yes =
		resize_output.addButton(QTStr("Yes"), QMessageBox::YesRole);
	resize_output.addButton(QTStr("No"), QMessageBox::NoRole);
	resize_output.setIcon(QMessageBox::Warning);
	resize_output.setWindowTitle(QTStr("ResizeOutputSizeOfSource"));
	resize_output.exec();

	if (resize_output.clickedButton() != Yes)
		return;

	OBSSource source = obs_sceneitem_get_source(GetCurrentSceneItem());

	int width = obs_source_get_width(source);
	int height = obs_source_get_height(source);

	config_set_uint(basicConfig, "Video", "BaseCX", width);
	config_set_uint(basicConfig, "Video", "BaseCY", height);
	config_set_uint(basicConfig, "Video", "OutputCX", width);
	config_set_uint(basicConfig, "Video", "OutputCY", height);

	ResetVideo();
	on_actionFitToScreen_triggered();
}

QAction *OBSBasic::AddDockWidget(QDockWidget *dock)
{
#if 0
	QAction *action = ui->viewMenuDocks->addAction(dock->windowTitle());
	action->setCheckable(true);
	assignDockToggle(dock, action);
	extraDocks.push_back(dock);

	bool lock = ui->lockUI->isChecked();
	QDockWidget::DockWidgetFeatures features =
		lock ? QDockWidget::NoDockWidgetFeatures
		     : QDockWidget::AllDockWidgetFeatures;

	dock->setFeatures(features);

	/* prune deleted docks */
	for (int i = extraDocks.size() - 1; i >= 0; i--) {
		if (!extraDocks[i]) {
			extraDocks.removeAt(i);
		}
	}
	return action;
#endif
    return nullptr;
}

OBSBasic *OBSBasic::Get()
{
	return reinterpret_cast<OBSBasic *>(App()->GetMainWindow());
}
#if 0
bool OBSBasic::StreamingActive()
{
	if (!outputHandler)
		return false;
	return outputHandler->StreamingActive();
}
#endif
bool OBSBasic::RecordingActive()
{
	if (!outputHandler)
		return false;
	return outputHandler->RecordingActive();
}

bool OBSBasic::ReplayBufferActive()
{
	if (!outputHandler)
		return false;
	return outputHandler->ReplayBufferActive();
}

SceneRenameDelegate::SceneRenameDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

void SceneRenameDelegate::setEditorData(QWidget *editor,
					const QModelIndex &index) const
{
	QStyledItemDelegate::setEditorData(editor, index);
	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
	if (lineEdit)
		lineEdit->selectAll();
}

bool SceneRenameDelegate::eventFilter(QObject *editor, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Escape) {
			QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
			if (lineEdit)
				lineEdit->undo();
		}
	}

	return QStyledItemDelegate::eventFilter(editor, event);
}

void OBSBasic::UpdatePatronJson(const QString &text, const QString &error)
{
	if (!error.isEmpty())
		return;

	patronJson = QT_TO_UTF8(text);
}

void OBSBasic::PauseRecording()
{
	if (!pause || !outputHandler || !outputHandler->fileOutput)
		return;

	obs_output_t *output = outputHandler->fileOutput;

	if (obs_output_pause(output, true)) {
		pause->setAccessibleName(QTStr("Basic.Main.UnpauseRecording"));
		pause->setToolTip(QTStr("Basic.Main.UnpauseRecording"));
		pause->blockSignals(true);
		pause->setChecked(true);
		pause->blockSignals(false);

		if (trayIcon)
			trayIcon->setIcon(QIcon(":/res/images/obs_paused.png"));

		os_atomic_set_bool(&recording_paused, true);

		if (api)
			api->on_event(OBS_FRONTEND_EVENT_RECORDING_PAUSED);

		if (os_atomic_load_bool(&replaybuf_active))
			ShowReplayBufferPauseWarning();
	}
}

void OBSBasic::UnpauseRecording()
{
	if (!pause || !outputHandler || !outputHandler->fileOutput)
		return;

	obs_output_t *output = outputHandler->fileOutput;

	if (obs_output_pause(output, false)) {
		pause->setAccessibleName(QTStr("Basic.Main.PauseRecording"));
		pause->setToolTip(QTStr("Basic.Main.PauseRecording"));
		pause->blockSignals(true);
		pause->setChecked(false);
		pause->blockSignals(false);

		if (trayIcon)
			trayIcon->setIcon(
				QIcon(":/res/images/tray_active.png"));

		os_atomic_set_bool(&recording_paused, false);

		if (api)
			api->on_event(OBS_FRONTEND_EVENT_RECORDING_UNPAUSED);
	}
}

void OBSBasic::PauseToggled()
{
	if (!pause || !outputHandler || !outputHandler->fileOutput)
		return;

	obs_output_t *output = outputHandler->fileOutput;
	bool enable = !obs_output_paused(output);

	if (enable)
		PauseRecording();
	else
		UnpauseRecording();
}

void OBSBasic::UpdatePause(bool activate)
{
	if (!activate || !outputHandler || !outputHandler->RecordingActive()) {
		pause.reset();
		return;
	}

	const char *mode = config_get_string(basicConfig, "Output", "Mode");
	bool adv = astrcmpi(mode, "Advanced") == 0;
	bool shared;

	if (adv) {
		const char *recType =
			config_get_string(basicConfig, "AdvOut", "RecType");

		if (astrcmpi(recType, "FFmpeg") == 0) {
			shared = config_get_bool(basicConfig, "AdvOut",
						 "FFOutputToFile");
		} else {
			const char *recordEncoder = config_get_string(
				basicConfig, "AdvOut", "RecEncoder");
			shared = astrcmpi(recordEncoder, "none") == 0;
		}
	} else {
		const char *quality = config_get_string(
			basicConfig, "SimpleOutput", "RecQuality");
		shared = strcmp(quality, "Stream") == 0;
	}

	if (!shared) {
		pause.reset(new QPushButton());
		pause->setAccessibleName(QTStr("Basic.Main.PauseRecording"));
		pause->setToolTip(QTStr("Basic.Main.PauseRecording"));
		pause->setCheckable(true);
		pause->setChecked(false);
		pause->setProperty("themeID",
				   QVariant(QStringLiteral("pauseIconSmall")));
		connect(pause.data(), &QAbstractButton::clicked, this,
			&OBSBasic::PauseToggled);
		ui->recordingLayout->addWidget(pause.data());
	} else {
		pause.reset();
	}
}

void OBSBasic::UpdateReplayBuffer(bool activate)
{
	if (!activate || !outputHandler ||
	    !outputHandler->ReplayBufferActive()) {
		replay.reset();
		return;
	}

	replay.reset(new QPushButton());
	replay->setAccessibleName(QTStr("Basic.Main.SaveReplay"));
	replay->setToolTip(QTStr("Basic.Main.SaveReplay"));
	replay->setCheckable(true);
	replay->setChecked(false);
	replay->setProperty("themeID",
			    QVariant(QStringLiteral("replayIconSmall")));
	connect(replay.data(), &QAbstractButton::clicked, this,
		&OBSBasic::ReplayBufferSave);
	replayLayout->addWidget(replay.data());
}

#define MBYTE (1024ULL * 1024ULL)
#define MBYTES_LEFT_STOP_REC 50ULL
#define MAX_BYTES_LEFT (MBYTES_LEFT_STOP_REC * MBYTE)

const char *OBSBasic::GetCurrentOutputPath()
{
	const char *path = nullptr;
	const char *mode = config_get_string(Config(), "Output", "Mode");

	if (strcmp(mode, "Advanced") == 0) {
		const char *advanced_mode =
			config_get_string(Config(), "AdvOut", "RecType");

		if (strcmp(advanced_mode, "FFmpeg") == 0) {
			path = config_get_string(Config(), "AdvOut",
						 "FFFilePath");
		} else {
			path = config_get_string(Config(), "AdvOut",
						 "RecFilePath");
		}
	} else {
		path = config_get_string(Config(), "SimpleOutput", "FilePath");
	}

	return path;
}

void OBSBasic::OutputPathInvalidMessage()
{
	blog(LOG_ERROR, "Recording stopped because of bad output path");

	OBSMessageBox::critical(this, QTStr("Output.BadPath.Title"),
				QTStr("Output.BadPath.Text"));
}

bool OBSBasic::OutputPathValid()
{
	const char *path = GetCurrentOutputPath();
	return path && *path && QDir(path).exists();
}

void OBSBasic::DiskSpaceMessage()
{
	blog(LOG_ERROR, "Recording stopped because of low disk space");

	OBSMessageBox::critical(this, QTStr("Output.RecordNoSpace.Title"),
				QTStr("Output.RecordNoSpace.Msg"));
}

bool OBSBasic::LowDiskSpace()
{
	const char *path;

	path = GetCurrentOutputPath();
	if (!path)
		return false;

	uint64_t num_bytes = os_get_free_disk_space(path);

	if (num_bytes < (MAX_BYTES_LEFT))
		return true;
	else
		return false;
}

void OBSBasic::CheckDiskSpaceRemaining()
{
	if (LowDiskSpace()) {
		StopRecording();
		StopReplayBuffer();

		DiskSpaceMessage();
	}
}

void OBSBasic::ScenesReordered(const QModelIndex &parent, int start, int end,
			       const QModelIndex &destination, int row)
{
	UNUSED_PARAMETER(parent);
	UNUSED_PARAMETER(start);
	UNUSED_PARAMETER(end);
	UNUSED_PARAMETER(destination);
	UNUSED_PARAMETER(row);

	OBSProjector::UpdateMultiviewProjectors();
}

void OBSBasic::ResetStatsHotkey()
{
	QList<OBSBasicStats *> list = findChildren<OBSBasicStats *>();

	foreach(OBSBasicStats * s, list) s->Reset();
}

void OBSBasic::on_customContextMenuRequested(const QPoint &pos)
{
	QWidget *widget = childAt(pos);
	const char *className = nullptr;
	if (widget != nullptr)
		className = widget->metaObject()->className();

	//if (!className || strstr(className, "Dock") != nullptr)
//		ui->viewMenuDocks->exec(mapToGlobal(pos));
}


void OBSBasic::UpdateVideoProperties(void *data, calldata_t *)
{
	QMetaObject::invokeMethod(static_cast<OBSBasic *>(data)->videoview,
				  "ReloadProperties");
}

void OBSBasic::UpdateAudioProperties(void *data, calldata_t *)
{
	QMetaObject::invokeMethod(static_cast<OBSBasic *>(data)->audioview,
				  "ReloadProperties");
}

void OBSBasic::SetControlProperties() // storm
{
    // storm
    //QLabel* lbl = new QLabel("test");
    //ui->verticalLayout->addWidget(lbl);
    // 1, source properties
    bool visible = true;
	OBSBasic *main = reinterpret_cast<OBSBasic *>(App()->GetMainWindow());
	OBSScene scene = main->GetCurrentScene();
	bool success = false;
	if (!scene)
    {
        // qInfo()<<"error";
		return ;
    }

	size_t idx = 0;
	const char *unversioned_type;
	const char *type;
	bool foundDeprecated = false;
	bool foundValues = false;

	while (obs_enum_input_types2(idx++, &type, &unversioned_type)) {
		const char *name = obs_source_get_display_name(type);
		uint32_t caps = obs_get_source_output_flags(type);

		if ((caps & OBS_SOURCE_CAP_DISABLED) != 0)
			continue;
		blog(LOG_DEBUG, "device name: %s type: %s", name, type);

		if ((caps & OBS_SOURCE_DEPRECATED) == 0) {
			// addSource(popup, unversioned_type, name);
            if(astrcmpi(name, "Video Capture Device") == 0 
                    || astrcmpi(name, "Audio Input Capture") == 0)
            {
                const char *id = type;
                obs_source_t *source = obs_get_source_by_name(name);
                if(!source)
                {
                    const char *v_id = obs_get_latest_input_type_id(id);
                    source = obs_source_create(v_id, name, NULL, nullptr);
                    if (source) {
                        AddSourceData data;
                        data.source = source;
                        data.visible = visible;

                        obs_enter_graphics();
                        obs_scene_atomic_update(scene, MyAddSource, &data);
                        obs_leave_graphics();

                        // newSource = source;

                        /* set monitoring if source monitors by default */
                        uint32_t flags = obs_source_get_output_flags(source);
                        if ((flags & OBS_SOURCE_MONITOR_BY_DEFAULT) != 0) {
                            obs_source_set_monitoring_type(
                                    source,
                                    OBS_MONITORING_TYPE_MONITOR_ONLY);
                        }

                        success = true;
                    }
                }
                if(astrcmpi(name, "Video Capture Device") == 0 )
                {
                    videosource = source;
                }
                else if( astrcmpi(name, "Audio Input Capture") == 0)
                {
                    audiosource = source;
                }
                obs_source_release(source);
            }
		} else {
			// addSource(deprecated, unversioned_type, name);
			foundDeprecated = true;
		}
		foundValues = true;
    }
    if(videosource)
    {
        videoremovedSignal=OBSSignal(obs_source_get_signal_handler(videosource), "remove",
                OBSBasic::SourceRemoved, this);
        videorenamedSignal=OBSSignal(obs_source_get_signal_handler(videosource), "rename",
                OBSBasic::SourceRenamed, this);
        videooldSettings=obs_data_create();
    }
    if(audiosource)
    {
        audioremovedSignal=OBSSignal(obs_source_get_signal_handler(audiosource), "remove",
                OBSBasic::SourceRemoved, this);
        audiorenamedSignal=OBSSignal(obs_source_get_signal_handler(audiosource), "rename",
                OBSBasic::SourceRenamed, this);
        audiooldSettings=obs_data_create();
    }
	int cx = 0;//// (int)config_get_int(App()->GlobalConfig(), "PropertiesWindow", "cx");
	int cy = 0;//(int)config_get_int(App()->GlobalConfig(), "PropertiesWindow", "cy");
	enum obs_source_type videotype = obs_source_get_type(videosource);
	enum obs_source_type audiotype = obs_source_get_type(audiosource);
#if 0
	// buttonBox->setObjectName(QStringLiteral("buttonBox")); // tmp
	if (cx > 400 && cy > 400)
		resize(cx, cy);
	else
		resize(720, 580);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif
	/* The OBSData constructor increments the reference once */
	obs_data_release(videooldSettings);
	obs_data_release(audiooldSettings);

	OBSData videosettings = obs_source_get_settings(videosource);
	obs_data_apply(videooldSettings, videosettings);
	obs_data_release(videosettings);

	OBSData audiosettings = obs_source_get_settings(audiosource);
	obs_data_apply(audiooldSettings, audiosettings);
	obs_data_release(audiosettings);

	videoview = new OBSPropertiesView(
		videosettings, videosource,
		(PropertiesReloadCallback)obs_source_properties,
		(PropertiesUpdateCallback)obs_source_update, 0, this);
	videoview->setMinimumHeight(150);

	audioview = new OBSPropertiesView(
		audiosettings, audiosource,
		(PropertiesReloadCallback)obs_source_properties,
		(PropertiesUpdateCallback)obs_source_update, 0, this);
	audioview->setMinimumHeight(150);

    //ui->verticalLayout->addWidget(videoview);
	//ui->verticalLayout->addWidget(audioview);
    ui->vcdDeviceVLayout->addWidget(videoview); // storm
	ui->acdVLayout->addWidget(audioview); // storm
	videoview->show();
	audioview->show();
	obs_source_inc_showing(videosource);
	obs_source_inc_showing(audiosource);

	videoupdatePropertiesSignal.Connect(obs_source_get_signal_handler(videosource),
				       "update_properties",
				       OBSBasic::UpdateVideoProperties,
				       this);
	audioupdatePropertiesSignal.Connect(obs_source_get_signal_handler(audiosource),
				       "update_properties",
				       OBSBasic::UpdateAudioProperties,
				       this);


    //2. video setting properties
	const char *val =
		config_get_string(Config(), "Video", "FPSCommon");

	int idxFPS = ui->fpsCommon->findText(val);
	if (idxFPS == -1)
		idxFPS = 4;
	ui->fpsCommon->setCurrentIndex(idxFPS);

    // 3, outputpath
	const char *path =
		config_get_string(Config(), "SimpleOutput", "FilePath");
	ui->simpleOutputPath->setText(path);
}

extern volatile bool recording_paused;
void OBSBasic::UpdateRecordTime()
{
	bool paused = os_atomic_load_bool(&recording_paused);
	if (!paused)
		totalRecordSeconds++;
	QString text;

	if (paused) {
		text = QStringLiteral("REC: PAUSED");
	} else {
		int seconds = totalRecordSeconds % 60;
		int totalMinutes = totalRecordSeconds / 60;
		int minutes = totalMinutes % 60;
		int hours = totalMinutes / 60;

		text = QString::asprintf("REC: %02d:%02d:%02d", hours, minutes,
					 seconds);
	}

	recordTime->setText(text);
	recordTime->setMinimumWidth(recordTime->width());
}

void Ui_OBSBasic::setupUi(OBSBasic *OBSBasic)
{
    if (OBSBasic->objectName().isEmpty())
        OBSBasic->setObjectName(QString::fromUtf8("OBSBasic"));
    // OBSBasic->resize(1079, 730);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(OBSBasic->sizePolicy().hasHeightForWidth());
    OBSBasic->setSizePolicy(sizePolicy);
    OBSBasic->setMinimumSize(QSize(0, 0));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/res/images/obs.png"), QSize(), QIcon::Normal, QIcon::Off);
    OBSBasic->setWindowIcon(icon);
    OBSBasic->setStyleSheet(QString::fromUtf8(""));
    // OBSBasic->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
    // new layout
    previewDisabledWidget = new QWidget();
    previewDisabledWidget->setObjectName(QString::fromUtf8("previewDisabledWidget"));
    enablePreviewButton = new QPushButton(previewDisabledWidget);
    enablePreviewButton->setObjectName(QString::fromUtf8("enablePreviewButton"));
    previewLayout = new QHBoxLayout();
    previewLayout->setSpacing(2);
    previewLayout->setObjectName(QString::fromUtf8("previewLayout"));
    previewLayout->addWidget(previewDisabledWidget);

    recordingLayout = new QHBoxLayout();
    recordingLayout->setSpacing(2);
    recordingLayout->setObjectName(QString::fromUtf8("recordingLayout"));
    recordingLayout->setContentsMargins(0, 0, 0, 0);

    scenes = new SceneTree();
    scenes->setObjectName(QString::fromUtf8("scenes"));
    sources = new SourceTree();
    sources->setObjectName(QString::fromUtf8("sources"));
    transitions = new QComboBox();
    transitions->setObjectName(QString::fromUtf8("transitions"));
    transitions->setMinimumSize(QSize(120, 0));
    transitionDuration = new QSpinBox();

#if 1
#if 0
    centralwidget = new QWidget(OBSBasic);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
	OBSBasic->setLayout(new QVBoxLayout(OBSBasic));
	OBSBasic->layout()->addWidget(centralwidget);
#endif
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);

    centralHorizontalLayout = new QHBoxLayout(OBSBasic);
    leftVerticalLayout = new QVBoxLayout;
    centralHorizontalLayout->addLayout(leftVerticalLayout);
        // preview = new QWidget();
        preview = new OBSBasicPreview(OBSBasic);
        leftVerticalLayout->addWidget(preview);
            preview->setObjectName(QString::fromUtf8("preview"));
            sizePolicy1.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
            preview->setSizePolicy(sizePolicy1);
            preview->setMinimumSize(QSize(32, 32));
            preview->setFocusPolicy(Qt::ClickFocus);
            preview->setContextMenuPolicy(Qt::CustomContextMenu);
        // hMixerScrollArea = new QWidget();
        hMixerScrollArea = new VScrollArea();
        leftVerticalLayout->addWidget(hMixerScrollArea);
            hMixerScrollArea->setObjectName(QString::fromUtf8("hMixerScrollArea"));
            hMixerScrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
            hMixerScrollArea->setFrameShape(QFrame::StyledPanel);
            hMixerScrollArea->setFrameShadow(QFrame::Sunken);
            hMixerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            hMixerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            hMixerScrollArea->setWidgetResizable(true);
            hVolumeWidgets = new QWidget();
            hVolumeWidgets->setObjectName(QString::fromUtf8("hVolumeWidgets"));
            hVolumeWidgets->setGeometry(QRect(0, 0, 71, 16));
            QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
            sizePolicy4.setHorizontalStretch(0);
            sizePolicy4.setVerticalStretch(0);
            sizePolicy4.setHeightForWidth(hVolumeWidgets->sizePolicy().hasHeightForWidth());
            hVolumeWidgets->setSizePolicy(sizePolicy4);
            hVolControlLayout = new QVBoxLayout(hVolumeWidgets);
            hVolControlLayout->setSpacing(0);
            hVolControlLayout->setObjectName(QString::fromUtf8("hVolControlLayout"));
            hVolControlLayout->setContentsMargins(0, 0, 0, 0);
            hMixerScrollArea->setWidget(hVolumeWidgets);

            vMixerScrollArea = new HScrollArea();
            vMixerScrollArea->setObjectName(QString::fromUtf8("vMixerScrollArea"));
            vMixerScrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
            vMixerScrollArea->setFrameShape(QFrame::StyledPanel);
            vMixerScrollArea->setFrameShadow(QFrame::Sunken);
            vMixerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            vMixerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            vMixerScrollArea->setWidgetResizable(true);
            vVolumeWidgets = new QWidget();
            vVolumeWidgets->setObjectName(QString::fromUtf8("vVolumeWidgets"));
            vVolumeWidgets->setGeometry(QRect(0, 0, 16, 28));
            QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Preferred);
            sizePolicy5.setHorizontalStretch(0);
            sizePolicy5.setVerticalStretch(0);
            sizePolicy5.setHeightForWidth(vVolumeWidgets->sizePolicy().hasHeightForWidth());
            vVolumeWidgets->setSizePolicy(sizePolicy5);
            vVolControlLayout = new QHBoxLayout(vVolumeWidgets);
            vVolControlLayout->setSpacing(0);
            vVolControlLayout->setObjectName(QString::fromUtf8("vVolControlLayout"));
            vVolControlLayout->setContentsMargins(0, 0, 0, 0);
            vMixerScrollArea->setWidget(vVolumeWidgets);
        //recordButton = new QPushButton(tr("recordButton"));
        recordButton = new RecordButton;
        leftVerticalLayout->addWidget(recordButton);
            //recordButton = new RecordButton();
            recordButton->setObjectName(QString::fromUtf8("recordButton"));
            recordButton->setEnabled(true);
            QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Fixed);
            sizePolicy7.setHorizontalStretch(0);
            sizePolicy7.setVerticalStretch(0);
            sizePolicy7.setHeightForWidth(recordButton->sizePolicy().hasHeightForWidth());
            recordButton->setSizePolicy(sizePolicy7);
            recordButton->setMinimumSize(QSize(130, 0));
            recordButton->setCheckable(true);
        //recordTime = new QLabel(tr("recordTime"));
        OBSBasic->recordTime = new QLabel();
        //leftVerticalLayout->addWidget(recordTime);
        leftVerticalLayout->addWidget(OBSBasic->recordTime);
            OBSBasic->recordTime->setText(QString("REC: 00:00:00"));
            //verticalLayout->addWidget(OBSBasic->recordTime);
            OBSBasic->recordTime->setAlignment(Qt::AlignRight);
            OBSBasic->recordTime->setAlignment(Qt::AlignVCenter);
            OBSBasic->recordTime->setIndent(20);
    rightVerticalLayout = new QVBoxLayout();
    centralHorizontalLayout->addLayout(rightVerticalLayout);
        captureSettingGBox = new QGroupBox(QTStr("Capture settings"));
        rightVerticalLayout->addWidget(captureSettingGBox);
            captureSettingVLayout = new QVBoxLayout;
            captureSettingGBox->setLayout(captureSettingVLayout);
                vcdLabel = new QLabel(QTStr("Video capture device:"));
                captureSettingVLayout->addWidget(vcdLabel);
                vcdSettingVLayout = new QVBoxLayout;
                captureSettingVLayout->addLayout(vcdSettingVLayout);
                    vcdDeviceVLayout = new QVBoxLayout;
                    vcdSettingVLayout->addLayout(vcdDeviceVLayout);
#if 0
                    resolutionVLayout = new QVBoxLayout;
                    vcdSettingVLayout->addLayout(resolutionVLayout);
                        resolutionLabel = new QLabel(QTStr("Resolution:"));
                        resolutionVLayout->addWidget(resolutionLabel);
                        resolutionComboBox = new QComboBox;
                        resolutionVLayout->addWidget(resolutionComboBox);
#endif
                    fpsVLayout = new QVBoxLayout;
                    vcdSettingVLayout->addLayout(fpsVLayout);
                        fpsLabel = new QLabel(QTStr("Frame rate: "));
                        //fpsVLayout->addWidget(fpsLabel); //tmp storm
                        fpsCommon = new QComboBox;
                        //fpsVLayout->addWidget(fpsCommon);
                            fpsCommon->addItem(QString::fromUtf8("10"));
                            fpsCommon->addItem(QString::fromUtf8("20"));
                            // fpsCommon->addItem(QString());
                            // fpsCommon->addItem(QString());
                            fpsCommon->addItem(QString::fromUtf8("29.97"));
                            fpsCommon->addItem(QString::fromUtf8("30"));
                            fpsCommon->addItem(QString::fromUtf8("48"));
                            // fpsCommon->addItem(QString());
                            fpsCommon->addItem(QString::fromUtf8("59.94"));
                            fpsCommon->addItem(QString::fromUtf8("60"));
                            fpsCommon->setObjectName(QString::fromUtf8("fpsCommon"));
                            fpsCommon->setCurrentText(QString::fromUtf8("30"));
                            OBSBasic->connect(fpsCommon, SIGNAL(currentIndexChanged(int)), OBSBasic, SLOT(changeFPS()));
                acdVLayout = new QVBoxLayout;
                captureSettingVLayout->addLayout(acdVLayout);
                    acdLabel = new QLabel(QTStr("Audio capture device: "));
                    acdVLayout->addWidget(acdLabel);
                    //acdComboBox = new QComboBox;
                    //captureSettingVLayout->addWidget(acdComboBox);
        saveSettingGBox = new QGroupBox(QTStr("Saving Captured Files"));
        rightVerticalLayout->addWidget(saveSettingGBox);
            saveSettingVLayout = new QVBoxLayout;
            saveSettingGBox->setLayout(saveSettingVLayout);
                savefileLabel = new QLabel(QTStr("Save files to: "));
                saveSettingVLayout->addWidget(savefileLabel);
                browseHLayout = new QHBoxLayout;
                saveSettingVLayout->addLayout(browseHLayout);
                    simpleOutputPath = new QLineEdit;
                    simpleOutputPath->setObjectName(QString::fromUtf8("simpleOutputPath"));
                    simpleOutputPath->setEnabled(true);
                    simpleOutputPath->setReadOnly(true);
                    browseHLayout->addWidget(simpleOutputPath);
                    // directoryComboBox = OBSBasic->createComboBox(QDir::currentPath());
                    //browseHLayout->addWidget(directoryComboBox);
                    //browseButton = OBSBasic->createButton(QTStr("Browse..."), SLOT(browse()));
                    //browseHLayout->addWidget(browseButton);
                    simpleOutputBrowse = new QPushButton();
                    browseHLayout->addWidget(simpleOutputBrowse);
                    simpleOutputBrowse->setObjectName(QString::fromUtf8("simpleOutputBrowse"));
                    simpleOutputBrowse->setEnabled(true);
                //viewfileButton = OBSBasic->createButton(QTStr("View record files"), SLOT(opendir()));
                viewfileButton = new QPushButton(QTStr("View record files"));
                saveSettingVLayout->addWidget(viewfileButton);
                viewfileButton->setObjectName(QString::fromUtf8("viewfileButton"));
	OBSBasic->setLayout(centralHorizontalLayout);
#endif
#if 0
    // old layout
#if 0
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/res/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionAddScene = new QAction(OBSBasic);
    actionAddScene->setObjectName(QString::fromUtf8("actionAddScene"));
    actionAddScene->setIcon(icon1);
    actionAddScene->setProperty("themeID", QVariant(QString::fromUtf8("addIconSmall")));
    actionAddSource = new QAction(OBSBasic);
    actionAddSource->setObjectName(QString::fromUtf8("actionAddSource"));
    actionAddSource->setIcon(icon1);
    actionAddSource->setProperty("themeID", QVariant(QString::fromUtf8("addIconSmall")));
    actionRemoveScene = new QAction(OBSBasic);
    actionRemoveScene->setObjectName(QString::fromUtf8("actionRemoveScene"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/res/images/list_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRemoveScene->setIcon(icon2);
    actionRemoveScene->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    actionRemoveScene->setProperty("themeID", QVariant(QString::fromUtf8("removeIconSmall")));
    actionRemoveSource = new QAction(OBSBasic);
    actionRemoveSource->setObjectName(QString::fromUtf8("actionRemoveSource"));
    actionRemoveSource->setIcon(icon2);
    actionRemoveSource->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    actionRemoveSource->setProperty("themeID", QVariant(QString::fromUtf8("removeIconSmall")));
    actionSourceProperties = new QAction(OBSBasic);
    actionSourceProperties->setObjectName(QString::fromUtf8("actionSourceProperties"));
    actionSourceProperties->setEnabled(true);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/res/images/properties.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSourceProperties->setIcon(icon3);
    actionSourceProperties->setProperty("themeID", QVariant(QString::fromUtf8("propertiesIconSmall")));
    actionSceneUp = new QAction(OBSBasic);
    actionSceneUp->setObjectName(QString::fromUtf8("actionSceneUp"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/res/images/up.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSceneUp->setIcon(icon4);
    actionSceneUp->setProperty("themeID", QVariant(QString::fromUtf8("upArrowIconSmall")));
    actionSourceUp = new QAction(OBSBasic);
    actionSourceUp->setObjectName(QString::fromUtf8("actionSourceUp"));
    actionSourceUp->setEnabled(true);
    actionSourceUp->setIcon(icon4);
    actionSourceUp->setProperty("themeID", QVariant(QString::fromUtf8("upArrowIconSmall")));
    actionSceneDown = new QAction(OBSBasic);
    actionSceneDown->setObjectName(QString::fromUtf8("actionSceneDown"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/res/images/down.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSceneDown->setIcon(icon5);
    actionSceneDown->setProperty("themeID", QVariant(QString::fromUtf8("downArrowIconSmall")));
    actionSourceDown = new QAction(OBSBasic);
    actionSourceDown->setObjectName(QString::fromUtf8("actionSourceDown"));
    actionSourceDown->setEnabled(true);
    actionSourceDown->setIcon(icon5);
    actionSourceDown->setProperty("themeID", QVariant(QString::fromUtf8("downArrowIconSmall")));
    actionShow_Recordings = new QAction(OBSBasic);
    actionShow_Recordings->setObjectName(QString::fromUtf8("actionShow_Recordings"));
    actionRemux = new QAction(OBSBasic);
    actionRemux->setObjectName(QString::fromUtf8("actionRemux"));
    action_Settings = new QAction(OBSBasic);
    action_Settings->setObjectName(QString::fromUtf8("action_Settings"));
    actionE_xit = new QAction(OBSBasic);
    actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
    actionShowLogs = new QAction(OBSBasic);
    actionShowLogs->setObjectName(QString::fromUtf8("actionShowLogs"));
    actionUploadLastLog = new QAction(OBSBasic);
    actionUploadLastLog->setObjectName(QString::fromUtf8("actionUploadLastLog"));
    actionUploadCurrentLog = new QAction(OBSBasic);
    actionUploadCurrentLog->setObjectName(QString::fromUtf8("actionUploadCurrentLog"));
    actionViewCurrentLog = new QAction(OBSBasic);
    actionViewCurrentLog->setObjectName(QString::fromUtf8("actionViewCurrentLog"));
    actionUndo = new QAction(OBSBasic);
    actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
    actionUndo->setEnabled(false);
    actionRedo = new QAction(OBSBasic);
    actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
    actionRedo->setEnabled(false);
    actionEditTransform = new QAction(OBSBasic);
    actionEditTransform->setObjectName(QString::fromUtf8("actionEditTransform"));
    actionCopyTransform = new QAction(OBSBasic);
    actionCopyTransform->setObjectName(QString::fromUtf8("actionCopyTransform"));
    actionPasteTransform = new QAction(OBSBasic);
    actionPasteTransform->setObjectName(QString::fromUtf8("actionPasteTransform"));
    actionPasteTransform->setEnabled(false);
    actionRotate90CW = new QAction(OBSBasic);
    actionRotate90CW->setObjectName(QString::fromUtf8("actionRotate90CW"));
    actionRotate90CCW = new QAction(OBSBasic);
    actionRotate90CCW->setObjectName(QString::fromUtf8("actionRotate90CCW"));
    actionRotate180 = new QAction(OBSBasic);
    actionRotate180->setObjectName(QString::fromUtf8("actionRotate180"));
    actionFitToScreen = new QAction(OBSBasic);
    actionFitToScreen->setObjectName(QString::fromUtf8("actionFitToScreen"));
    actionStretchToScreen = new QAction(OBSBasic);
    actionStretchToScreen->setObjectName(QString::fromUtf8("actionStretchToScreen"));
    actionResetTransform = new QAction(OBSBasic);
    actionResetTransform->setObjectName(QString::fromUtf8("actionResetTransform"));
    actionCenterToScreen = new QAction(OBSBasic);
    actionCenterToScreen->setObjectName(QString::fromUtf8("actionCenterToScreen"));
    actionVerticalCenter = new QAction(OBSBasic);
    actionVerticalCenter->setObjectName(QString::fromUtf8("actionVerticalCenter"));
    actionHorizontalCenter = new QAction(OBSBasic);
    actionHorizontalCenter->setObjectName(QString::fromUtf8("actionHorizontalCenter"));
    actionFlipHorizontal = new QAction(OBSBasic);
    actionFlipHorizontal->setObjectName(QString::fromUtf8("actionFlipHorizontal"));
    actionFlipVertical = new QAction(OBSBasic);
    actionFlipVertical->setObjectName(QString::fromUtf8("actionFlipVertical"));
    actionMoveUp = new QAction(OBSBasic);
    actionMoveUp->setObjectName(QString::fromUtf8("actionMoveUp"));
    actionMoveDown = new QAction(OBSBasic);
    actionMoveDown->setObjectName(QString::fromUtf8("actionMoveDown"));
    actionMoveToTop = new QAction(OBSBasic);
    actionMoveToTop->setObjectName(QString::fromUtf8("actionMoveToTop"));
    actionMoveToBottom = new QAction(OBSBasic);
    actionMoveToBottom->setObjectName(QString::fromUtf8("actionMoveToBottom"));
    actionCheckForUpdates = new QAction(OBSBasic);
    actionCheckForUpdates->setObjectName(QString::fromUtf8("actionCheckForUpdates"));
    actionInteract = new QAction(OBSBasic);
    actionInteract->setObjectName(QString::fromUtf8("actionInteract"));
    actionAdvAudioProperties = new QAction(OBSBasic);
    actionAdvAudioProperties->setObjectName(QString::fromUtf8("actionAdvAudioProperties"));
    actionWebsite = new QAction(OBSBasic);
    actionWebsite->setObjectName(QString::fromUtf8("actionWebsite"));
    actionNewSceneCollection = new QAction(OBSBasic);
    actionNewSceneCollection->setObjectName(QString::fromUtf8("actionNewSceneCollection"));
    actionDupSceneCollection = new QAction(OBSBasic);
    actionDupSceneCollection->setObjectName(QString::fromUtf8("actionDupSceneCollection"));
    actionRenameSceneCollection = new QAction(OBSBasic);
    actionRenameSceneCollection->setObjectName(QString::fromUtf8("actionRenameSceneCollection"));
    actionRemoveSceneCollection = new QAction(OBSBasic);
    actionRemoveSceneCollection->setObjectName(QString::fromUtf8("actionRemoveSceneCollection"));
    actionImportSceneCollection = new QAction(OBSBasic);
    actionImportSceneCollection->setObjectName(QString::fromUtf8("actionImportSceneCollection"));
    actionExportSceneCollection = new QAction(OBSBasic);
    actionExportSceneCollection->setObjectName(QString::fromUtf8("actionExportSceneCollection"));
    actionNewProfile = new QAction(OBSBasic);
    actionNewProfile->setObjectName(QString::fromUtf8("actionNewProfile"));
    actionDupProfile = new QAction(OBSBasic);
    actionDupProfile->setObjectName(QString::fromUtf8("actionDupProfile"));
    actionRenameProfile = new QAction(OBSBasic);
    actionRenameProfile->setObjectName(QString::fromUtf8("actionRenameProfile"));
    actionRemoveProfile = new QAction(OBSBasic);
    actionRemoveProfile->setObjectName(QString::fromUtf8("actionRemoveProfile"));
    actionImportProfile = new QAction(OBSBasic);
    actionImportProfile->setObjectName(QString::fromUtf8("actionImportProfile"));
    actionExportProfile = new QAction(OBSBasic);
    actionExportProfile->setObjectName(QString::fromUtf8("actionExportProfile"));
    actionShowSettingsFolder = new QAction(OBSBasic);
    actionShowSettingsFolder->setObjectName(QString::fromUtf8("actionShowSettingsFolder"));
    actionShowProfileFolder = new QAction(OBSBasic);
    actionShowProfileFolder->setObjectName(QString::fromUtf8("actionShowProfileFolder"));
    actionAlwaysOnTop = new QAction(OBSBasic);
    actionAlwaysOnTop->setObjectName(QString::fromUtf8("actionAlwaysOnTop"));
    actionAlwaysOnTop->setCheckable(true);
    toggleListboxToolbars = new QAction(OBSBasic);
    toggleListboxToolbars->setObjectName(QString::fromUtf8("toggleListboxToolbars"));
    toggleListboxToolbars->setCheckable(true);
    toggleListboxToolbars->setChecked(true);
    toggleStatusBar = new QAction(OBSBasic);
    toggleStatusBar->setObjectName(QString::fromUtf8("toggleStatusBar"));
    toggleStatusBar->setCheckable(true);
    toggleStatusBar->setChecked(true);
    actionLockPreview = new QAction(OBSBasic);
    actionLockPreview->setObjectName(QString::fromUtf8("actionLockPreview"));
    actionLockPreview->setCheckable(true);
    actionScaleWindow = new QAction(OBSBasic);
    actionScaleWindow->setObjectName(QString::fromUtf8("actionScaleWindow"));
    actionScaleWindow->setCheckable(true);
    actionScaleCanvas = new QAction(OBSBasic);
    actionScaleCanvas->setObjectName(QString::fromUtf8("actionScaleCanvas"));
    actionScaleCanvas->setCheckable(true);
    actionScaleOutput = new QAction(OBSBasic);
    actionScaleOutput->setObjectName(QString::fromUtf8("actionScaleOutput"));
    actionScaleOutput->setCheckable(true);
    actionPasteDup = new QAction(OBSBasic);
    actionPasteDup->setObjectName(QString::fromUtf8("actionPasteDup"));
    autoConfigure2 = new QAction(OBSBasic);
    autoConfigure2->setObjectName(QString::fromUtf8("autoConfigure2"));
    autoConfigure = new QAction(OBSBasic);
    autoConfigure->setObjectName(QString::fromUtf8("autoConfigure"));
    stats = new QAction(OBSBasic);
    stats->setObjectName(QString::fromUtf8("stats"));
    resetUI = new QAction(OBSBasic);
    resetUI->setObjectName(QString::fromUtf8("resetUI"));
    lockUI = new QAction(OBSBasic);
    lockUI->setObjectName(QString::fromUtf8("lockUI"));
    lockUI->setCheckable(true);
    lockUI->setChecked(true);
    toggleScenes = new QAction(OBSBasic);
    toggleScenes->setObjectName(QString::fromUtf8("toggleScenes"));
    toggleScenes->setCheckable(true);
    toggleScenes->setChecked(true);
    toggleSources = new QAction(OBSBasic);
    toggleSources->setObjectName(QString::fromUtf8("toggleSources"));
    toggleSources->setCheckable(true);
    toggleSources->setChecked(true);
    toggleMixer = new QAction(OBSBasic);
    toggleMixer->setObjectName(QString::fromUtf8("toggleMixer"));
    toggleMixer->setCheckable(true);
    toggleMixer->setChecked(true);
#if 0
    toggleTransitions = new QAction(OBSBasic);
    toggleTransitions->setObjectName(QString::fromUtf8("toggleTransitions"));
    toggleTransitions->setCheckable(true);
    toggleTransitions->setChecked(true);
#endif
    toggleControls = new QAction(OBSBasic);
    toggleControls->setObjectName(QString::fromUtf8("toggleControls"));
    toggleControls->setCheckable(true);
    toggleControls->setChecked(true);
    actionHelpPortal = new QAction(OBSBasic);
    actionHelpPortal->setObjectName(QString::fromUtf8("actionHelpPortal"));
    actionShowCrashLogs = new QAction(OBSBasic);
    actionShowCrashLogs->setObjectName(QString::fromUtf8("actionShowCrashLogs"));
    actionUploadLastCrashLog = new QAction(OBSBasic);
    actionUploadLastCrashLog->setObjectName(QString::fromUtf8("actionUploadLastCrashLog"));
    actionDiscord = new QAction(OBSBasic);
    actionDiscord->setObjectName(QString::fromUtf8("actionDiscord"));
    toggleStats = new QAction(OBSBasic);
    toggleStats->setObjectName(QString::fromUtf8("toggleStats"));
    toggleStats->setCheckable(true);
    toggleStats->setChecked(true);
    actionShowAbout = new QAction(OBSBasic);
    actionShowAbout->setObjectName(QString::fromUtf8("actionShowAbout"));
    toggleSourceIcons = new QAction(OBSBasic);
    toggleSourceIcons->setObjectName(QString::fromUtf8("toggleSourceIcons"));
    toggleSourceIcons->setCheckable(true);
    toggleSourceIcons->setChecked(true);
#endif
    centralwidget = new QWidget(OBSBasic);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    previewLayout = new QHBoxLayout();
    previewLayout->setSpacing(2);
    previewLayout->setObjectName(QString::fromUtf8("previewLayout"));
    previewDisabledWidget = new QWidget(centralwidget);
    previewDisabledWidget->setObjectName(QString::fromUtf8("previewDisabledWidget"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(previewDisabledWidget->sizePolicy().hasHeightForWidth());
    previewDisabledWidget->setSizePolicy(sizePolicy1);
    verticalLayout_7 = new QVBoxLayout(previewDisabledWidget);
    verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_7->addItem(verticalSpacer_2);

    label = new QLabel(previewDisabledWidget);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy2);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout_7->addWidget(label);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_2);

    enablePreviewButton = new QPushButton(previewDisabledWidget);
    enablePreviewButton->setObjectName(QString::fromUtf8("enablePreviewButton"));

    horizontalLayout->addWidget(enablePreviewButton);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_3);


    verticalLayout_7->addLayout(horizontalLayout);

    verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_7->addItem(verticalSpacer_3);


    previewLayout->addWidget(previewDisabledWidget);

    previewTextLayout = new QVBoxLayout();
    previewTextLayout->setSpacing(0);
    previewTextLayout->setObjectName(QString::fromUtf8("previewTextLayout"));
#if 0
    previewLabel = new QLabel(centralwidget);
    previewLabel->setObjectName(QString::fromUtf8("previewLabel"));
    previewLabel->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

    previewTextLayout->addWidget(previewLabel);
#endif
    preview = new OBSBasicPreview(centralwidget);
    preview->setObjectName(QString::fromUtf8("preview"));
    sizePolicy1.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
    preview->setSizePolicy(sizePolicy1);
    preview->setMinimumSize(QSize(32, 32));
    preview->setFocusPolicy(Qt::ClickFocus);
    preview->setContextMenuPolicy(Qt::CustomContextMenu);

    previewTextLayout->addWidget(preview);


    previewLayout->addLayout(previewTextLayout);


    horizontalLayout_2->addLayout(previewLayout);


    verticalLayout->addLayout(horizontalLayout_2);

    // OBSBasic->setCentralWidget(centralwidget);
	OBSBasic->setLayout(new QVBoxLayout(OBSBasic));
	OBSBasic->layout()->addWidget(centralwidget);
#if 0
    menubar = new QMenuBar(OBSBasic);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 1079, 22));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    menuBasic_MainMenu_Help = new QMenu(menubar);
    menuBasic_MainMenu_Help->setObjectName(QString::fromUtf8("menuBasic_MainMenu_Help"));
    menuLogFiles = new QMenu(menuBasic_MainMenu_Help);
    menuLogFiles->setObjectName(QString::fromUtf8("menuLogFiles"));
    menuCrashLogs = new QMenu(menuBasic_MainMenu_Help);
    menuCrashLogs->setObjectName(QString::fromUtf8("menuCrashLogs"));
    menuBasic_MainMenu_Edit = new QMenu(menubar);
    menuBasic_MainMenu_Edit->setObjectName(QString::fromUtf8("menuBasic_MainMenu_Edit"));
    actionCopySource = new QAction(menuBasic_MainMenu_Edit);
    actionCopySource->setObjectName(QString::fromUtf8("actionCopySource"));
    actionPasteRef = new QAction(menuBasic_MainMenu_Edit);
    actionPasteRef->setObjectName(QString::fromUtf8("actionPasteRef"));
    actionPasteRef->setEnabled(false);
    actionCopyFilters = new QAction(menuBasic_MainMenu_Edit);
    actionCopyFilters->setObjectName(QString::fromUtf8("actionCopyFilters"));
    actionPasteFilters = new QAction(menuBasic_MainMenu_Edit);
    actionPasteFilters->setObjectName(QString::fromUtf8("actionPasteFilters"));
    actionPasteFilters->setEnabled(false);
    transformMenu = new QMenu(menuBasic_MainMenu_Edit);
    transformMenu->setObjectName(QString::fromUtf8("transformMenu"));
    orderMenu = new QMenu(menuBasic_MainMenu_Edit);
    orderMenu->setObjectName(QString::fromUtf8("orderMenu"));
    scalingMenu = new QMenu(menuBasic_MainMenu_Edit);
    scalingMenu->setObjectName(QString::fromUtf8("scalingMenu"));
    profileMenu = new QMenu(menubar);
    profileMenu->setObjectName(QString::fromUtf8("profileMenu"));
    sceneCollectionMenu = new QMenu(menubar);
    sceneCollectionMenu->setObjectName(QString::fromUtf8("sceneCollectionMenu"));
    viewMenu = new QMenu(menubar);
    viewMenu->setObjectName(QString::fromUtf8("viewMenu"));
    actionFullscreenInterface = new QAction(viewMenu);
    actionFullscreenInterface->setObjectName(QString::fromUtf8("actionFullscreenInterface"));
    viewMenuToolbars = new QMenu(viewMenu);
    viewMenuToolbars->setObjectName(QString::fromUtf8("viewMenuToolbars"));
    viewMenuDocks = new QMenu(viewMenu);
    viewMenuDocks->setObjectName(QString::fromUtf8("viewMenuDocks"));
    menuTools = new QMenu(menubar);
    menuTools->setObjectName(QString::fromUtf8("menuTools"));
#endif
#if 0
    OBSBasic->setMenuBar(menubar);
    statusbar = new OBSBasicStatusBar(OBSBasic);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    OBSBasic->setStatusBar(statusbar);
#endif
    OBSBasic->recordTime = new QLabel();
    OBSBasic->recordTime->setText(QString("REC: 00:00:00"));
    verticalLayout->addWidget(OBSBasic->recordTime);
	OBSBasic->recordTime->setAlignment(Qt::AlignRight);
	OBSBasic->recordTime->setAlignment(Qt::AlignVCenter);
	OBSBasic->recordTime->setIndent(20);
#if 0
    scenesDock = new OBSDock(OBSBasic);
    scenesDock->setObjectName(QString::fromUtf8("scenesDock"));
    scenesDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
#endif
    dockWidgetContents_2 = new QWidget();
    dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
    verticalLayout_6 = new QVBoxLayout(dockWidgetContents_2);
    verticalLayout_6->setSpacing(0);
    verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
    verticalLayout_6->setContentsMargins(0, 0, 0, 0);
    scenesFrame = new QFrame(dockWidgetContents_2);
    scenesFrame->setObjectName(QString::fromUtf8("scenesFrame"));
    sizePolicy1.setHeightForWidth(scenesFrame->sizePolicy().hasHeightForWidth());
    scenesFrame->setSizePolicy(sizePolicy1);
    scenesFrame->setMinimumSize(QSize(160, 0));
    scenesFrame->setFrameShape(QFrame::StyledPanel);
    scenesFrame->setFrameShadow(QFrame::Sunken);
    verticalLayout_12 = new QVBoxLayout(scenesFrame);
    verticalLayout_12->setSpacing(0);
    verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
    verticalLayout_12->setContentsMargins(0, 0, 0, 0);
    scenes = new SceneTree(scenesFrame);
    scenes->setObjectName(QString::fromUtf8("scenes"));
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(scenes->sizePolicy().hasHeightForWidth());
    scenes->setSizePolicy(sizePolicy3);
    scenes->setContextMenuPolicy(Qt::CustomContextMenu);
    scenes->setFrameShape(QFrame::NoFrame);
    scenes->setFrameShadow(QFrame::Plain);
    scenes->setProperty("showDropIndicator", QVariant(true));
    scenes->setDragEnabled(true);
    scenes->setDragDropMode(QAbstractItemView::InternalMove);
    scenes->setDefaultDropAction(Qt::TargetMoveAction);

    verticalLayout_12->addWidget(scenes);
#if 0
    scenesToolbar = new QToolBar(scenesFrame);
    scenesToolbar->setObjectName(QString::fromUtf8("scenesToolbar"));
    scenesToolbar->setIconSize(QSize(16, 16));
    scenesToolbar->setFloatable(false);

    verticalLayout_12->addWidget(scenesToolbar);
#endif
    scenesFixedSizeHSpacer = new QSpacerItem(150, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    verticalLayout_12->addItem(scenesFixedSizeHSpacer);


    verticalLayout_6->addWidget(scenesFrame);
#if 0
    scenesDock->setWidget(dockWidgetContents_2);
    OBSBasic->addDockWidget(Qt::BottomDockWidgetArea, scenesDock);
#endif
#if 0
    sourcesDock = new OBSDock(OBSBasic);
    sourcesDock->setObjectName(QString::fromUtf8("sourcesDock"));
    sourcesDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
#endif
    dockWidgetContents_6 = new QWidget();
    dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
    verticalLayout_5 = new QVBoxLayout(dockWidgetContents_6);
    verticalLayout_5->setSpacing(0);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    verticalLayout_5->setContentsMargins(0, 0, 0, 0);
    sourcesFrame = new QFrame(dockWidgetContents_6);
    sourcesFrame->setObjectName(QString::fromUtf8("sourcesFrame"));
    sizePolicy1.setHeightForWidth(sourcesFrame->sizePolicy().hasHeightForWidth());
    sourcesFrame->setSizePolicy(sizePolicy1);
    sourcesFrame->setMinimumSize(QSize(160, 0));
    sourcesFrame->setFrameShape(QFrame::StyledPanel);
    sourcesFrame->setFrameShadow(QFrame::Sunken);
    verticalLayout_17 = new QVBoxLayout(sourcesFrame);
    verticalLayout_17->setSpacing(0);
    verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
    verticalLayout_17->setContentsMargins(0, 0, 0, 0);
    sources = new SourceTree(sourcesFrame);
    sources->setObjectName(QString::fromUtf8("sources"));
    sizePolicy3.setHeightForWidth(sources->sizePolicy().hasHeightForWidth());
    sources->setSizePolicy(sizePolicy3);
    sources->setContextMenuPolicy(Qt::CustomContextMenu);
    sources->setFrameShape(QFrame::NoFrame);
    sources->setProperty("showDropIndicator", QVariant(true));
    sources->setDragEnabled(true);
    sources->setDragDropMode(QAbstractItemView::InternalMove);
    sources->setDefaultDropAction(Qt::TargetMoveAction);
    sources->setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalLayout_17->addWidget(sources);
    //verticalLayout->addWidget(sources);
#if 0
    sourcesToolbar = new QToolBar(sourcesFrame);
    sourcesToolbar->setObjectName(QString::fromUtf8("sourcesToolbar"));
    sourcesToolbar->setIconSize(QSize(16, 16));
    sourcesToolbar->setFloatable(false);

    verticalLayout_17->addWidget(sourcesToolbar);
#endif
    sourcesFixedSizeHSpacer = new QSpacerItem(150, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

    verticalLayout_17->addItem(sourcesFixedSizeHSpacer);


    verticalLayout_5->addWidget(sourcesFrame);
#if 0
    sourcesDock->setWidget(dockWidgetContents_6);
    OBSBasic->addDockWidget(Qt::BottomDockWidgetArea, sourcesDock);
    mixerDock = new OBSDock(OBSBasic);
    mixerDock->setObjectName(QString::fromUtf8("mixerDock"));
    mixerDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockWidgetContents_7 = new QWidget();
    dockWidgetContents_7->setObjectName(QString::fromUtf8("dockWidgetContents_7"));
    verticalLayout_4 = new QVBoxLayout(dockWidgetContents_7);
    verticalLayout_4->setSpacing(0);
    verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
    verticalLayout_4->setContentsMargins(0, 0, 0, 0);
    stackedMixerArea = new QStackedWidget(dockWidgetContents_7);
    stackedMixerArea->setObjectName(QString::fromUtf8("stackedMixerArea"));
#endif
    hMixerScrollArea = new VScrollArea();
    hMixerScrollArea->setObjectName(QString::fromUtf8("hMixerScrollArea"));
    hMixerScrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
    hMixerScrollArea->setFrameShape(QFrame::StyledPanel);
    hMixerScrollArea->setFrameShadow(QFrame::Sunken);
    hMixerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    hMixerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    hMixerScrollArea->setWidgetResizable(true);
    hVolumeWidgets = new QWidget();
    hVolumeWidgets->setObjectName(QString::fromUtf8("hVolumeWidgets"));
    hVolumeWidgets->setGeometry(QRect(0, 0, 71, 16));
    QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(hVolumeWidgets->sizePolicy().hasHeightForWidth());
    hVolumeWidgets->setSizePolicy(sizePolicy4);
    hVolControlLayout = new QVBoxLayout(hVolumeWidgets);
    hVolControlLayout->setSpacing(0);
    hVolControlLayout->setObjectName(QString::fromUtf8("hVolControlLayout"));
    hVolControlLayout->setContentsMargins(0, 0, 0, 0);
    hMixerScrollArea->setWidget(hVolumeWidgets);
    //stackedMixerArea->addWidget(hMixerScrollArea);
    verticalLayout->addWidget(hMixerScrollArea); //storm
    vMixerScrollArea = new HScrollArea();
    vMixerScrollArea->setObjectName(QString::fromUtf8("vMixerScrollArea"));
    vMixerScrollArea->setContextMenuPolicy(Qt::CustomContextMenu);
    vMixerScrollArea->setFrameShape(QFrame::StyledPanel);
    vMixerScrollArea->setFrameShadow(QFrame::Sunken);
    vMixerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vMixerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    vMixerScrollArea->setWidgetResizable(true);
    vVolumeWidgets = new QWidget();
    vVolumeWidgets->setObjectName(QString::fromUtf8("vVolumeWidgets"));
    vVolumeWidgets->setGeometry(QRect(0, 0, 16, 28));
    QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(vVolumeWidgets->sizePolicy().hasHeightForWidth());
    vVolumeWidgets->setSizePolicy(sizePolicy5);
    vVolControlLayout = new QHBoxLayout(vVolumeWidgets);
    vVolControlLayout->setSpacing(0);
    vVolControlLayout->setObjectName(QString::fromUtf8("vVolControlLayout"));
    vVolControlLayout->setContentsMargins(0, 0, 0, 0);
    vMixerScrollArea->setWidget(vVolumeWidgets);
    //stackedMixerArea->addWidget(vMixerScrollArea);
    // verticalLayout->addWidget(vMixerScrollArea); // tmp storm 先取消掉

    //verticalLayout_4->addWidget(stackedMixerArea);

#if 0
    mixerDock->setWidget(dockWidgetContents_7);
    OBSBasic->addDockWidget(Qt::BottomDockWidgetArea, mixerDock);
    transitionsDock = new OBSDock(OBSBasic);
    transitionsDock->setObjectName(QString::fromUtf8("transitionsDock"));
    transitionsDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockWidgetContents_5 = new QWidget();
    dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
    verticalLayout_3 = new QVBoxLayout(dockWidgetContents_5);
    verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
    verticalLayout_3->setContentsMargins(4, 4, 4, 4);
    transitionsContainer = new QWidget(dockWidgetContents_5);
    transitionsContainer->setObjectName(QString::fromUtf8("transitionsContainer"));
    verticalLayout_2 = new QVBoxLayout(transitionsContainer);
    verticalLayout_2->setSpacing(4);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(1, 1, 1, 2);
    transitions = new QComboBox(transitionsContainer);
#endif
    transitions = new QComboBox();
    transitions->setObjectName(QString::fromUtf8("transitions"));
    transitions->setMinimumSize(QSize(120, 0));
#if 0
    verticalLayout_2->addWidget(transitions);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setSpacing(4);
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_4->addItem(horizontalSpacer);

    transitionAdd = new QPushButton(transitionsContainer);
    transitionAdd->setObjectName(QString::fromUtf8("transitionAdd"));
    QSizePolicy sizePolicy6(QSizePolicy::Maximum, QSizePolicy::Maximum);
    sizePolicy6.setHorizontalStretch(0);
    sizePolicy6.setVerticalStretch(0);
    sizePolicy6.setHeightForWidth(transitionAdd->sizePolicy().hasHeightForWidth());
    transitionAdd->setSizePolicy(sizePolicy6);
    transitionAdd->setMaximumSize(QSize(22, 22));
    transitionAdd->setText(QString::fromUtf8(""));
    transitionAdd->setIcon(icon1);
    transitionAdd->setFlat(true);
    transitionAdd->setProperty("themeID", QVariant(QString::fromUtf8("addIconSmall")));

    horizontalLayout_4->addWidget(transitionAdd);

    transitionRemove = new QPushButton(transitionsContainer);
    transitionRemove->setObjectName(QString::fromUtf8("transitionRemove"));
    sizePolicy6.setHeightForWidth(transitionRemove->sizePolicy().hasHeightForWidth());
    transitionRemove->setSizePolicy(sizePolicy6);
    transitionRemove->setMaximumSize(QSize(22, 22));
    transitionRemove->setText(QString::fromUtf8(""));
    transitionRemove->setIcon(icon2);
    transitionRemove->setFlat(true);
    transitionRemove->setProperty("themeID", QVariant(QString::fromUtf8("removeIconSmall")));

    horizontalLayout_4->addWidget(transitionRemove);

    transitionProps = new QPushButton(transitionsContainer);
    transitionProps->setObjectName(QString::fromUtf8("transitionProps"));
    sizePolicy6.setHeightForWidth(transitionProps->sizePolicy().hasHeightForWidth());
    transitionProps->setSizePolicy(sizePolicy6);
    transitionProps->setMaximumSize(QSize(22, 22));
    transitionProps->setText(QString::fromUtf8(""));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/res/images/configuration21_16.png"), QSize(), QIcon::Normal, QIcon::Off);
    transitionProps->setIcon(icon6);
    transitionProps->setFlat(true);
    transitionProps->setProperty("themeID", QVariant(QString::fromUtf8("configIconSmall")));

    horizontalLayout_4->addWidget(transitionProps);


    verticalLayout_2->addLayout(horizontalLayout_4);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(4);
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    transitionDurationLabel = new QLabel(transitionsContainer);
    transitionDurationLabel->setObjectName(QString::fromUtf8("transitionDurationLabel"));
    sizePolicy5.setHeightForWidth(transitionDurationLabel->sizePolicy().hasHeightForWidth());
    transitionDurationLabel->setSizePolicy(sizePolicy5);

    horizontalLayout_3->addWidget(transitionDurationLabel);
    transitionDuration = new QSpinBox(transitionsContainer);
    // transitionDuration = new QSpinBox();
    transitionDuration->setObjectName(QString::fromUtf8("transitionDuration"));
    transitionDuration->setMinimum(2);
    transitionDuration->setMaximum(10000);
    transitionDuration->setSingleStep(50);
    transitionDuration->setValue(300);
    horizontalLayout_3->addWidget(transitionDuration);


    verticalLayout_2->addLayout(horizontalLayout_3);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout_2->addItem(verticalSpacer);


    verticalLayout_3->addWidget(transitionsContainer);

    transitionsDock->setWidget(dockWidgetContents_5);
    OBSBasic->addDockWidget(Qt::BottomDockWidgetArea, transitionsDock);
    controlsDock = new OBSDock(OBSBasic);
    controlsDock->setObjectName(QString::fromUtf8("controlsDock"));
    controlsDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    controlsDockContents = new QWidget();
    controlsDockContents->setObjectName(QString::fromUtf8("controlsDockContents"));
    buttonsVLayout = new QVBoxLayout(controlsDockContents);
    buttonsVLayout->setSpacing(2);
    buttonsVLayout->setObjectName(QString::fromUtf8("buttonsVLayout"));
    buttonsVLayout->setContentsMargins(4, 4, 4, 4);
    streamButton = new QPushButton(controlsDockContents);
    streamButton->setObjectName(QString::fromUtf8("streamButton"));
    streamButton->setEnabled(true);
#else
    controlsDockContents = new QWidget();
    controlsDockContents->setObjectName(QString::fromUtf8("controlsDockContents"));
#endif
    QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy7.setHorizontalStretch(0);
    sizePolicy7.setVerticalStretch(0);
#if 0
    sizePolicy7.setHeightForWidth(streamButton->sizePolicy().hasHeightForWidth());
    streamButton->setSizePolicy(sizePolicy7);
    streamButton->setCheckable(true);

    buttonsVLayout->addWidget(streamButton);
#endif
    recordingLayout = new QHBoxLayout();
    recordingLayout->setSpacing(2);
    recordingLayout->setObjectName(QString::fromUtf8("recordingLayout"));
    recordingLayout->setContentsMargins(0, 0, 0, 0);
    recordButton = new RecordButton(controlsDockContents);
    //recordButton = new RecordButton();
    recordButton->setObjectName(QString::fromUtf8("recordButton"));
    recordButton->setEnabled(true);
    sizePolicy7.setHeightForWidth(recordButton->sizePolicy().hasHeightForWidth());
    recordButton->setSizePolicy(sizePolicy7);
    recordButton->setMinimumSize(QSize(130, 0));
    recordButton->setCheckable(true);

    recordingLayout->addWidget(recordButton);


    // buttonsVLayout->addLayout(recordingLayout); // storm
    verticalLayout->addLayout(recordingLayout); // storm
#if 0
    modeSwitch = new QPushButton(controlsDockContents);
    modeSwitch->setObjectName(QString::fromUtf8("modeSwitch"));
    modeSwitch->setCheckable(true);

    buttonsVLayout->addWidget(modeSwitch);
    settingsButton = new QPushButton(controlsDockContents);
    settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
    sizePolicy7.setHeightForWidth(settingsButton->sizePolicy().hasHeightForWidth());
    settingsButton->setSizePolicy(sizePolicy7);

    buttonsVLayout->addWidget(settingsButton);

    exitButton = new QPushButton(controlsDockContents);
    exitButton->setObjectName(QString::fromUtf8("exitButton"));
    sizePolicy7.setHeightForWidth(exitButton->sizePolicy().hasHeightForWidth());
    exitButton->setSizePolicy(sizePolicy7);

    buttonsVLayout->addWidget(exitButton);

    expVSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    buttonsVLayout->addItem(expVSpacer);
#endif

    //controlsDock->setWidget(controlsDockContents);
    //OBSBasic->addDockWidget(Qt::BottomDockWidgetArea, controlsDock);
#if QT_CONFIG(shortcut)
    //transitionDurationLabel->setBuddy(transitionDuration);
#endif // QT_CONFIG(shortcut)
#if 0
    preview->addAction(actionRemoveSource);
    menubar->addAction(menu_File->menuAction());
    menubar->addAction(menuBasic_MainMenu_Edit->menuAction());
    menubar->addAction(viewMenu->menuAction());
    menubar->addAction(profileMenu->menuAction());
    menubar->addAction(sceneCollectionMenu->menuAction());
    menubar->addAction(menuTools->menuAction());
    menubar->addAction(menuBasic_MainMenu_Help->menuAction());
    menu_File->addAction(actionShow_Recordings);
    menu_File->addAction(actionRemux);
    menu_File->addSeparator();
    menu_File->addAction(action_Settings);
    menu_File->addAction(actionShowSettingsFolder);
    menu_File->addAction(actionShowProfileFolder);
    menu_File->addSeparator();
    menu_File->addAction(actionAlwaysOnTop);
    menu_File->addSeparator();
    menu_File->addAction(actionE_xit);
    menuBasic_MainMenu_Help->addAction(actionHelpPortal);
    menuBasic_MainMenu_Help->addAction(actionWebsite);
    menuBasic_MainMenu_Help->addAction(actionDiscord);
    menuBasic_MainMenu_Help->addSeparator();
    menuBasic_MainMenu_Help->addAction(menuLogFiles->menuAction());
    menuBasic_MainMenu_Help->addAction(menuCrashLogs->menuAction());
    menuBasic_MainMenu_Help->addSeparator();
    menuBasic_MainMenu_Help->addAction(actionCheckForUpdates);
    menuBasic_MainMenu_Help->addAction(actionShowAbout);
    menuBasic_MainMenu_Help->addSeparator();
    menuLogFiles->addAction(actionShowLogs);
    menuLogFiles->addAction(actionUploadCurrentLog);
    menuLogFiles->addAction(actionUploadLastLog);
    menuLogFiles->addAction(actionViewCurrentLog);
    menuCrashLogs->addAction(actionShowCrashLogs);
    menuCrashLogs->addAction(actionUploadLastCrashLog);
    menuBasic_MainMenu_Edit->addAction(actionCopySource);
    menuBasic_MainMenu_Edit->addAction(actionPasteRef);
    menuBasic_MainMenu_Edit->addAction(actionPasteDup);
    menuBasic_MainMenu_Edit->addSeparator();
    menuBasic_MainMenu_Edit->addAction(actionCopyFilters);
    menuBasic_MainMenu_Edit->addAction(actionPasteFilters);
    menuBasic_MainMenu_Edit->addSeparator();
    menuBasic_MainMenu_Edit->addAction(transformMenu->menuAction());
    menuBasic_MainMenu_Edit->addAction(orderMenu->menuAction());
    menuBasic_MainMenu_Edit->addAction(scalingMenu->menuAction());
    menuBasic_MainMenu_Edit->addAction(actionLockPreview);
    menuBasic_MainMenu_Edit->addSeparator();
    menuBasic_MainMenu_Edit->addAction(actionAdvAudioProperties);
    transformMenu->addAction(actionEditTransform);
    transformMenu->addAction(actionCopyTransform);
    transformMenu->addAction(actionPasteTransform);
    transformMenu->addAction(actionResetTransform);
    transformMenu->addSeparator();
    transformMenu->addAction(actionRotate90CW);
    transformMenu->addAction(actionRotate90CCW);
    transformMenu->addAction(actionRotate180);
    transformMenu->addSeparator();
    transformMenu->addAction(actionFlipHorizontal);
    transformMenu->addAction(actionFlipVertical);
    transformMenu->addSeparator();
    transformMenu->addAction(actionFitToScreen);
    transformMenu->addAction(actionStretchToScreen);
    transformMenu->addAction(actionCenterToScreen);
    transformMenu->addAction(actionVerticalCenter);
    transformMenu->addAction(actionHorizontalCenter);
    orderMenu->addAction(actionMoveUp);
    orderMenu->addAction(actionMoveDown);
    orderMenu->addSeparator();
    orderMenu->addAction(actionMoveToTop);
    orderMenu->addAction(actionMoveToBottom);
    scalingMenu->addAction(actionScaleWindow);
    scalingMenu->addAction(actionScaleCanvas);
    scalingMenu->addAction(actionScaleOutput);
    profileMenu->addAction(actionNewProfile);
    profileMenu->addAction(actionDupProfile);
    profileMenu->addAction(actionRenameProfile);
    profileMenu->addAction(actionRemoveProfile);
    profileMenu->addAction(actionImportProfile);
    profileMenu->addAction(actionExportProfile);
    profileMenu->addSeparator();
    sceneCollectionMenu->addAction(actionNewSceneCollection);
    sceneCollectionMenu->addAction(actionDupSceneCollection);
    sceneCollectionMenu->addAction(actionRenameSceneCollection);
    sceneCollectionMenu->addAction(actionRemoveSceneCollection);
    sceneCollectionMenu->addAction(actionImportSceneCollection);
    sceneCollectionMenu->addAction(actionExportSceneCollection);
    sceneCollectionMenu->addSeparator();
    viewMenu->addAction(actionFullscreenInterface);
    viewMenu->addSeparator();
    viewMenu->addAction(viewMenuDocks->menuAction());
    viewMenu->addAction(viewMenuToolbars->menuAction());
    viewMenu->addAction(toggleSourceIcons);
    viewMenu->addAction(toggleStatusBar);
    viewMenu->addSeparator();
    viewMenu->addAction(stats);
    viewMenuToolbars->addAction(toggleListboxToolbars);
    viewMenuDocks->addAction(resetUI);
    viewMenuDocks->addAction(lockUI);
    viewMenuDocks->addSeparator();
    viewMenuDocks->addAction(toggleScenes);
    viewMenuDocks->addAction(toggleSources);
    viewMenuDocks->addAction(toggleMixer);
    //viewMenuDocks->addAction(toggleTransitions);
    viewMenuDocks->addAction(toggleControls);
    viewMenuDocks->addAction(toggleStats);
    menuTools->addAction(autoConfigure);
    menuTools->addSeparator();
    scenes->addAction(actionRemoveScene);
    //scenesToolbar->addAction(actionAddScene);
    scenesToolbar->addAction(actionRemoveScene);
    scenesToolbar->addSeparator();
    scenesToolbar->addAction(actionSceneUp);
    scenesToolbar->addAction(actionSceneDown);
    sources->addAction(actionRemoveSource);
    sourcesToolbar->addAction(actionAddSource);
    sourcesToolbar->addAction(actionRemoveSource);
    sourcesToolbar->addAction(actionSourceProperties);
    sourcesToolbar->addSeparator();
    sourcesToolbar->addAction(actionSourceUp);
    sourcesToolbar->addAction(actionSourceDown);
#endif
    // old layout
#endif
    retranslateUi(OBSBasic);
    //QObject::connect(actionE_xit, SIGNAL(triggered()), OBSBasic, SLOT(close()));
    // QObject::connect(exitButton, SIGNAL(clicked()), OBSBasic, SLOT(close()));

    QMetaObject::connectSlotsByName(OBSBasic);
}

void Ui_OBSBasic::retranslateUi(OBSBasic *OBSBasic)
{
    OBSBasic->setWindowTitle(QCoreApplication::translate("OBSBasic", ".MainWindow", nullptr));
    simpleOutputBrowse->setText(QCoreApplication::translate("OBSBasicSettings", "Browse", nullptr));
#if 0
    //actionAddScene->setText(QCoreApplication::translate("OBSBasic", "Add", nullptr));
    actionAddSource->setText(QCoreApplication::translate("OBSBasic", "Add", nullptr));
    actionRemoveScene->setText(QCoreApplication::translate("OBSBasic", "Remove", nullptr));
#if QT_CONFIG(shortcut)
    actionRemoveScene->setShortcut(QCoreApplication::translate("OBSBasic", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
    actionRemoveSource->setText(QCoreApplication::translate("OBSBasic", "Remove", nullptr));
#if QT_CONFIG(shortcut)
    actionRemoveSource->setShortcut(QCoreApplication::translate("OBSBasic", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
    actionSourceProperties->setText(QCoreApplication::translate("OBSBasic", "Properties", nullptr));
    actionSceneUp->setText(QCoreApplication::translate("OBSBasic", "MoveUp", nullptr));
    actionSourceUp->setText(QCoreApplication::translate("OBSBasic", "MoveUp", nullptr));
    actionSceneDown->setText(QCoreApplication::translate("OBSBasic", "MoveDown", nullptr));
    actionSourceDown->setText(QCoreApplication::translate("OBSBasic", "MoveDown", nullptr));
    actionShow_Recordings->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.ShowRecordings", nullptr));
    actionRemux->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.Remux", nullptr));
    action_Settings->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.Settings", nullptr));
    actionE_xit->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.Exit", nullptr));
    actionShowLogs->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Logs.ShowLogs", nullptr));
    actionUploadLastLog->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Logs.UploadLastLog", nullptr));
    actionUploadCurrentLog->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Logs.UploadCurrentLog", nullptr));
    actionViewCurrentLog->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Logs.ViewCurrentLog", nullptr));
    actionUndo->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Undo", nullptr));
    actionRedo->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Redo", nullptr));
    actionEditTransform->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.EditTransform", nullptr));
#if QT_CONFIG(shortcut)
    actionEditTransform->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
    actionCopyTransform->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.CopyTransform", nullptr));
    actionPasteTransform->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.PasteTransform", nullptr));
    actionRotate90CW->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.Rotate90CW", nullptr));
    actionRotate90CCW->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.Rotate90CCW", nullptr));
    actionRotate180->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.Rotate180", nullptr));
    actionFitToScreen->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.FitToScreen", nullptr));
#if QT_CONFIG(shortcut)
    actionFitToScreen->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
    actionStretchToScreen->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.StretchToScreen", nullptr));
#if QT_CONFIG(shortcut)
    actionStretchToScreen->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
    actionResetTransform->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.ResetTransform", nullptr));
#if QT_CONFIG(shortcut)
    actionResetTransform->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
    actionCenterToScreen->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.CenterToScreen", nullptr));
#if QT_CONFIG(shortcut)
    actionCenterToScreen->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
    actionVerticalCenter->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.VerticalCenter", nullptr));
    actionHorizontalCenter->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.HorizontalCenter", nullptr));
    actionFlipHorizontal->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.FlipHorizontal", nullptr));
    actionFlipVertical->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform.FlipVertical", nullptr));
    actionMoveUp->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Order.MoveUp", nullptr));
#if QT_CONFIG(shortcut)
    actionMoveUp->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+Up", nullptr));
#endif // QT_CONFIG(shortcut)
    actionMoveDown->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Order.MoveDown", nullptr));
#if QT_CONFIG(shortcut)
    actionMoveDown->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+Down", nullptr));
#endif // QT_CONFIG(shortcut)
    actionMoveToTop->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Order.MoveToTop", nullptr));
#if QT_CONFIG(shortcut)
    actionMoveToTop->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+Home", nullptr));
#endif // QT_CONFIG(shortcut)
    actionMoveToBottom->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Order.MoveToBottom", nullptr));
#if QT_CONFIG(shortcut)
    actionMoveToBottom->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+End", nullptr));
#endif // QT_CONFIG(shortcut)
    actionCheckForUpdates->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.CheckForUpdates", nullptr));
    actionInteract->setText(QCoreApplication::translate("OBSBasic", "Interact", nullptr));
    actionAdvAudioProperties->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.AdvAudio", nullptr));
    actionWebsite->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Website", nullptr));
    actionNewSceneCollection->setText(QCoreApplication::translate("OBSBasic", "New", nullptr));
    actionDupSceneCollection->setText(QCoreApplication::translate("OBSBasic", "Duplicate", nullptr));
    actionRenameSceneCollection->setText(QCoreApplication::translate("OBSBasic", "Rename", nullptr));
    actionRemoveSceneCollection->setText(QCoreApplication::translate("OBSBasic", "Remove", nullptr));
    actionImportSceneCollection->setText(QCoreApplication::translate("OBSBasic", "Import", nullptr));
    actionExportSceneCollection->setText(QCoreApplication::translate("OBSBasic", "Export", nullptr));
    actionNewProfile->setText(QCoreApplication::translate("OBSBasic", "New", nullptr));
    actionDupProfile->setText(QCoreApplication::translate("OBSBasic", "Duplicate", nullptr));
    actionRenameProfile->setText(QCoreApplication::translate("OBSBasic", "Rename", nullptr));
    actionRemoveProfile->setText(QCoreApplication::translate("OBSBasic", "Remove", nullptr));
    actionImportProfile->setText(QCoreApplication::translate("OBSBasic", "Import", nullptr));
    actionExportProfile->setText(QCoreApplication::translate("OBSBasic", "Export", nullptr));
    actionShowSettingsFolder->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.ShowSettingsFolder", nullptr));
    actionShowProfileFolder->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File.ShowProfileFolder", nullptr));
    actionAlwaysOnTop->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.AlwaysOnTop", nullptr));
    toggleListboxToolbars->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Toolbars.Listboxes", nullptr));
    // toggleStatusBar->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.StatusBar", nullptr));
    actionLockPreview->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.LockPreview", nullptr));
    actionScaleWindow->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Scale.Window", nullptr));
    actionScaleCanvas->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Scale.Canvas", nullptr));
    actionScaleOutput->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Scale.Output", nullptr));
    actionPasteDup->setText(QCoreApplication::translate("OBSBasic", "PasteDuplicate", nullptr));
    autoConfigure2->setText(QCoreApplication::translate("OBSBasic", "Basic.AutoConfig", nullptr));
    autoConfigure->setText(QCoreApplication::translate("OBSBasic", "Basic.AutoConfig", nullptr));
    stats->setText(QCoreApplication::translate("OBSBasic", "Basic.Stats", nullptr));
    resetUI->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Docks.ResetUI", nullptr));
    lockUI->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Docks.LockUI", nullptr));
    toggleScenes->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.Scenes", nullptr));
    toggleSources->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.Sources", nullptr));
    toggleMixer->setText(QCoreApplication::translate("OBSBasic", "Mixer", nullptr));
    //toggleTransitions->setText(QCoreApplication::translate("OBSBasic", "Basic.SceneTransitions", nullptr));
    toggleControls->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.Controls", nullptr));
    actionHelpPortal->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.HelpPortal", nullptr));
    actionShowCrashLogs->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.CrashLogs.ShowLogs", nullptr));
    actionUploadLastCrashLog->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.CrashLogs.UploadLastLog", nullptr));
    actionDiscord->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Discord", nullptr));
    toggleStats->setText(QCoreApplication::translate("OBSBasic", "Basic.Stats", nullptr));
    actionShowAbout->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.About", nullptr));
    toggleSourceIcons->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.SourceIcons", nullptr));
#endif
    //label->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.PreviewDisabled", nullptr));
    enablePreviewButton->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.PreviewConextMenu.Enable", nullptr));
#if 0
    // previewLabel->setText(QCoreApplication::translate("OBSBasic", "StudioMode.Preview", nullptr));
    menu_File->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.File", nullptr));
    menuBasic_MainMenu_Help->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help", nullptr));
    menuLogFiles->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.Logs", nullptr));
    menuCrashLogs->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Help.CrashLogs", nullptr));
    menuBasic_MainMenu_Edit->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit", nullptr));
    actionCopySource->setText(QCoreApplication::translate("OBSBasic", "Copy", nullptr));
#if QT_CONFIG(shortcut)
    actionCopySource->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
    actionPasteRef->setText(QCoreApplication::translate("OBSBasic", "PasteReference", nullptr));
    actionPasteRef->setIconText(QCoreApplication::translate("OBSBasic", "PasteReference", nullptr));
#if QT_CONFIG(tooltip)
    actionPasteRef->setToolTip(QCoreApplication::translate("OBSBasic", "PasteReference", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
    actionPasteRef->setShortcut(QCoreApplication::translate("OBSBasic", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
    actionCopyFilters->setText(QCoreApplication::translate("OBSBasic", "Copy.Filters", nullptr));
    actionPasteFilters->setText(QCoreApplication::translate("OBSBasic", "Paste.Filters", nullptr));
    transformMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Transform", nullptr));
    orderMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Order", nullptr));
    scalingMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Edit.Scale", nullptr));
    profileMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Profile", nullptr));
    sceneCollectionMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.SceneCollection", nullptr));
    viewMenu->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View", nullptr));
    actionFullscreenInterface->setText(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Fullscreen.Interface", nullptr));
#if QT_CONFIG(shortcut)
    actionFullscreenInterface->setShortcut(QCoreApplication::translate("OBSBasic", "F11", nullptr));
#endif // QT_CONFIG(shortcut)
    viewMenuToolbars->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Toolbars", nullptr));
    viewMenuDocks->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.View.Docks", nullptr));
    menuTools->setTitle(QCoreApplication::translate("OBSBasic", "Basic.MainMenu.Tools", nullptr));
    // scenesDock->setWindowTitle(QCoreApplication::translate("OBSBasic", "Basic.Main.Scenes", nullptr));
    //sourcesDock->setWindowTitle(QCoreApplication::translate("OBSBasic", "Basic.Main.Sources", nullptr));
    //mixerDock->setWindowTitle(QCoreApplication::translate("OBSBasic", "Mixer", nullptr));
    // transitionsDock->setWindowTitle(QCoreApplication::translate("OBSBasic", "Basic.SceneTransitions", nullptr));
#endif
#if QT_CONFIG(accessibility)
    transitions->setAccessibleName(QCoreApplication::translate("OBSBasic", "Transition", nullptr));
#endif // QT_CONFIG(accessibility)
#if 0
#if QT_CONFIG(tooltip)
    transitionAdd->setToolTip(QCoreApplication::translate("OBSBasic", "Basic.AddTransition", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(accessibility)
    transitionAdd->setAccessibleName(QCoreApplication::translate("OBSBasic", "Basic.AddTransition", nullptr));
#endif // QT_CONFIG(accessibility)
#if QT_CONFIG(tooltip)
    transitionRemove->setToolTip(QCoreApplication::translate("OBSBasic", "Basic.RemoveTransition", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(accessibility)
    transitionRemove->setAccessibleName(QCoreApplication::translate("OBSBasic", "Basic.RemoveTransition", nullptr));
#endif // QT_CONFIG(accessibility)
#if QT_CONFIG(tooltip)
    transitionProps->setToolTip(QCoreApplication::translate("OBSBasic", "Basic.TransitionProperties", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(accessibility)
    transitionProps->setAccessibleName(QCoreApplication::translate("OBSBasic", "Basic.TransitionProperties", nullptr));
#endif // QT_CONFIG(accessibility)
    transitionDurationLabel->setText(QCoreApplication::translate("OBSBasic", "Basic.TransitionDuration", nullptr));
#if QT_CONFIG(accessibility)
    transitionDuration->setAccessibleName(QCoreApplication::translate("OBSBasic", "Basic.TransitionDuration", nullptr));
#endif // QT_CONFIG(accessibility)
    transitionDuration->setSuffix(QCoreApplication::translate("OBSBasic", " ms", nullptr));
#endif
    //controlsDock->setWindowTitle(QCoreApplication::translate("OBSBasic", "Basic.Main.Controls", nullptr));
    // streamButton->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.StartStreaming", nullptr));
    recordButton->setText(QCoreApplication::translate("OBSBasic", "Basic.Main.StartRecording", nullptr));
    // modeSwitch->setText(QCoreApplication::translate("OBSBasic", "Basic.TogglePreviewProgramMode", nullptr));
    //settingsButton->setText(QCoreApplication::translate("OBSBasic", "Settings", nullptr));
    //exitButton->setText(QCoreApplication::translate("OBSBasic", "Exit", nullptr));
} // retranslateUi

QPushButton *OBSBasic::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

QComboBox *OBSBasic::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void OBSBasic::browse()
{
#if 0
    QString directory = QFileDialog::getExistingDirectory(this,
            tr("Find Files"), QDir::currentPath());
    if (!directory.isEmpty()) {
        ui->directoryComboBox->addItem(directory);
        ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->currentIndex() + 1);
    }
#endif
}

void OBSBasic::opendir()
{
    QString directory = QFileDialog::getExistingDirectory(this,
            QTStr("Find Files"), QDir::currentPath());
}

void OBSBasic::changeFPS()
{
    config_set_string(basicConfig, "Video", "FPSCommon",
            QT_TO_UTF8(ui->fpsCommon->currentText()));
    ResetVideo();
}

void OBSBasic::on_simpleOutputBrowse_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(
		this, QTStr("Basic.Settings.Output.SelectDirectory"),
		ui->simpleOutputPath->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty())
		return;

	ui->simpleOutputPath->setText(dir);
	// update SaveEdit(ui->simpleOutputPath, "SimpleOutput", "FilePath");
    config_set_string(basicConfig, "SimpleOutput", "FilePath",
            QT_TO_UTF8(ui->simpleOutputPath->text()));
}

void OBSBasic::on_viewfileButton_clicked()
{
    QString filePath;
    if(vsFilePaths.count()>0)
    {
        filePath=vsFilePaths.back();
    }
    else
    {
        filePath=ui->simpleOutputPath->text();
    }
    const QFileInfo fileInfo(filePath);
#ifdef __UBUNTU__
#elif TARGET_OS_MAC
    {
        QStringList scriptArgs;
        scriptArgs << QLatin1String("-e")
                   << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                          .arg(fileInfo.canonicalFilePath());
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
        scriptArgs.clear();
        scriptArgs << QLatin1String("-e")
                   << QLatin1String("tell application \"Finder\" to activate");
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    }
#else
    {
        QStringList param;
        if (!fileInfo.isDir())
        {
            param << QLatin1String("/select,");
        }
        param << QDir::toNativeSeparators(fileInfo.canonicalFilePath());
        // MProcess::startDetached("explorer.exe", param);
        QProcess::startDetached("explorer.exe", param);
    }
#endif
}

