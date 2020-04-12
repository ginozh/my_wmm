/***********************************************************************
 * File : RecordDialog.cpp
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2020-04-11     storm   1.0    created
 * 
 ***********************************************************************
 */

#include <QCloseEvent>
#include <QScreen>
#include <QWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLayout>
#include <QDebug>

//#include "obs-app.hpp"
#include "RecordDialog.hpp"
#include "qt-wrappers.hpp"
#include "util/profiler.hpp"
#include "util/util.hpp"
#include "util/dstr.h"
#include "util/platform.h"
#include "window-basic-main-outputs.hpp"
#include "properties-view.hpp"

using namespace std;

bool remuxAfterRecord = false;
string remuxFilename;

RecordDialog::RecordDialog(QWidget *parent)
	: QDialog(parent),
	  preview(new OBSQTDisplay(this))
{
    //1, 初始化obs
    //1.1
	if (!obs_startup("en-US", nullptr, nullptr)) // storm
    {
        qInfo()<<"error Couldn't create OBS";
		// throw "Couldn't create OBS";
        return;
    }
	int ret;
    // 1.2 ResetAudio()
    {
        struct obs_audio_info ai;
        // UI/window-basic-main.cpp config_set_default_uint
        ai.samples_per_sec = 44100;
        ai.speakers = SPEAKERS_STEREO;
        ret=obs_reset_audio(&ai);
    }
    //1.3 ResetVideo()
    {
        // UI/window-basic-main.cpp config_set_default_uint
        struct obs_video_info ovi;
        ovi.fps_num=30;
        ovi.fps_den=1;
        //ovi.graphics_module = App()->GetRenderModule();
        ovi.graphics_module = "libobs-opengl.so";
        ovi.base_width = 1920;
        ovi.base_height = 1080;
        ovi.output_width = 1280; // 同timeline
        ovi.output_height = 720; 
        //ovi.output_format = GetVideoFormatFromName(colorFormat); // NV12
        ovi.output_format = VIDEO_FORMAT_NV12;
        // ovi.colorspace = astrcmpi(colorSpace, "601") == 0 ? VIDEO_CS_601 : VIDEO_CS_709;
        ovi.colorspace = VIDEO_CS_601;
        //ovi.range = astrcmpi(colorRange, "Full") == 0 ? VIDEO_RANGE_FULL : VIDEO_RANGE_PARTIAL;
        ovi.range = VIDEO_RANGE_PARTIAL;
        //ovi.adapter = config_get_uint(App()->GlobalConfig(), "Video", "AdapterIdx"); // tmp storm
        ovi.gpu_conversion = true;
        // ovi.scale_type = GetScaleType(basicConfig); //tmp storm
        ovi.scale_type = OBS_SCALE_BICUBIC;
        ret=obs_reset_video(&ovi);
        //if (IS_WIN32 && ret != OBS_VIDEO_SUCCESS) // tmp storm
    }
	switch (ret) {
	case OBS_VIDEO_MODULE_NOT_FOUND:
        qInfo()<<"error Failed to initialize video:  Graphics module not found";
		//throw "Failed to initialize video:  Graphics module not found";
	case OBS_VIDEO_NOT_SUPPORTED:
        qInfo()<<"error UNSUPPORTED_ERROR";
		//throw UNSUPPORTED_ERROR;
	case OBS_VIDEO_INVALID_PARAM:
        qInfo()<<"error Failed to initialize video:  Invalid parameters";
		//throw "Failed to initialize video:  Invalid parameters";
	default:
		if (ret != OBS_VIDEO_SUCCESS)
        {
            qInfo()<<"error UNKNOWN_ERROR";
			//throw UNKNOWN_ERROR;
        }
	}
    // 1.4
	obs_load_all_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_log_loaded_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_post_load_modules();
    //
	// InitBasicConfigDefaults2(); // 设置编码encoder
	// config_set_default_string(basicConfig, "SimpleOutput", "StreamEncoder", useNV ? SIMPLE_ENCODER_NVENC : SIMPLE_ENCODER_X264);
	// config_set_default_string(basicConfig, "SimpleOutput", "RecEncoder", useNV ? SIMPLE_ENCODER_NVENC : SIMPLE_ENCODER_X264);
    // 1.5 init render vertex
	InitPrimitives(); 
    // 1.6 ResetOutputs()
    {
        const char *mode = "Simple";//config_get_string(basicConfig, "Output", "Mode");
        bool advOut = astrcmpi(mode, "Advanced") == 0;

        if (!outputHandler || !outputHandler->Active()) {
            outputHandler.reset();
            outputHandler.reset(advOut ? CreateAdvancedOutputHandler(this)
                    : CreateSimpleOutputHandler(this));
#if 0
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
                ui->buttonsVLayout->insertLayout(2, replayLayout);
            }

            if (sysTrayReplayBuffer)
                sysTrayReplayBuffer->setEnabled(
                        !!outputHandler->replayBuffer);
#endif
        } else {
            outputHandler->Update();
        }
    }
    {
        char savePath[512]="./Untitled.json"; // "/Users/user/Library/Application Support/obs-studio/basic/scenes/Untitled.json"
		//Load(savePath);
    }
    //2, 初始化界面
	preview->setMinimumSize(20, 150);
	preview->setSizePolicy(
		QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	// Create a QSplitter to keep a unified workflow here.
	windowSplitter = new QSplitter(Qt::Orientation::Vertical, this);
	windowSplitter->addWidget(preview);
	////windowSplitter->addWidget(view);
	windowSplitter->setChildrenCollapsible(false);
	//windowSplitter->setSizes(QList<int>({ 16777216, 150 }));
	windowSplitter->setStretchFactor(0, 3);
	windowSplitter->setStretchFactor(1, 1);

	setLayout(new QVBoxLayout(this));
	layout()->addWidget(windowSplitter);

	setWindowTitle(QT_UTF8("Record video"));
    //
    //3, 获取摄像头源(Video Capture Device)、麦克风源(Audio Input Capture)
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
                }
                if(astrcmpi(name, "Video Capture Device") == 0 )
                {
                    videosource = source;
                }
                else if( astrcmpi(name, "Audio Input Capture") == 0)
                {
                    audiosource = source;
                }
            }
		} else {
			// addSource(deprecated, unversioned_type, name);
			foundDeprecated = true;
		}
		foundValues = true;
    }
}
RecordDialog::~RecordDialog()
{
    //1, 释放obs
    //2, 释放videoSource
}
void RecordDialog::InitPrimitives()
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

string GenerateSpecifiedFilename(const char *extension, bool noSpace,
				 const char *format)
{
	// OBSBasic *main = reinterpret_cast<OBSBasic *>(App()->GetMainWindow());
	bool autoRemux = false;// config_get_bool(main->Config(), "Video", "AutoRemux");

	if ((strcmp(extension, "mp4") == 0) && autoRemux)
		extension = "mkv";

	BPtr<char> filename =
		os_generate_formatted_filename(extension, !noSpace, format);

	remuxFilename = string(filename);
	remuxAfterRecord = autoRemux;

	return string(filename);
}
