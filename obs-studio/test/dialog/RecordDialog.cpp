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

RecordDialog::RecordDialog(QWidget *parent)
	: QDialog(parent),
	  preview(new OBSQTDisplay(this))
{
    //1, 初始化obs
	if (!obs_startup("en-US", nullptr, nullptr)) // storm
    {
        qInfo()<<"error Couldn't create OBS";
		// throw "Couldn't create OBS";
        return;
    }
	int ret;
    //ResetAudio()
    {
        struct obs_audio_info ai;
        // UI/window-basic-main.cpp config_set_default_uint
        ai.samples_per_sec = 44100;
        ai.speakers = SPEAKERS_STEREO;
        ret=obs_reset_audio(&ai);
    }
    // ResetVideo()
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
	obs_load_all_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_log_loaded_modules();
	blog(LOG_INFO, "---------------------------------");
	obs_post_load_modules();
    //
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
    //3, 获取摄像头源、麦克风源
}
RecordDialog::~RecordDialog()
{
    //1, 释放obs
}
