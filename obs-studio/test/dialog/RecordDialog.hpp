/***********************************************************************
 * File : RecordDialog.hpp
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2020-04-11     storm   1.0    created
 * 
 ***********************************************************************
 */

#ifndef  RECORDDIALOG_INC
#define  RECORDDIALOG_INC

#include <QDialog>
#include <QDialogButtonBox>
#include <QPointer>
#include <QSplitter>
#include "qt-display.hpp"
#include <obs.hpp>
// window-basic-main.hpp =>
#define SIMPLE_ENCODER_X264 "x264"
#define SIMPLE_ENCODER_X264_LOWCPU "x264_lowcpu"
#define SIMPLE_ENCODER_QSV "qsv"
#define SIMPLE_ENCODER_NVENC "nvenc"
#define SIMPLE_ENCODER_AMD "amd"

extern bool remuxAfterRecord;
extern std::string remuxFilename;

struct BasicOutputHandler;
class OBSPropertiesView;

inline const char *Str(const char *lookup)
{
	// return App()->GetString(lookup); //storm tmp
	return lookup;
}
#define QTStr(lookupVal) QString::fromUtf8(Str(lookupVal))

std::string GenerateSpecifiedFilename(const char *extension, bool noSpace,
				      const char *format);

class RecordDialog : public QDialog {
	Q_OBJECT
public:
    RecordDialog(QWidget *parent);
    ~RecordDialog();
private:
	void InitPrimitives();
	static void SourceRemoved(void *data, calldata_t *params);
	static void SourceRenamed(void *data, calldata_t *params);
	static void UpdateVideoProperties(void *data, calldata_t *params);
	static void UpdateAudioProperties(void *data, calldata_t *params);
	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);
	//static void DrawTransitionPreview(void *data, uint32_t cx, uint32_t cy);
private:
	QPointer<OBSQTDisplay> preview;
	QSplitter *windowSplitter;


	gs_vertbuffer_t *box = nullptr;
	gs_vertbuffer_t *boxLeft = nullptr;
	gs_vertbuffer_t *boxTop = nullptr;
	gs_vertbuffer_t *boxRight = nullptr;
	gs_vertbuffer_t *boxBottom = nullptr;
	gs_vertbuffer_t *circle = nullptr;

	std::unique_ptr<BasicOutputHandler> outputHandler;

    // video
    OBSSource videosource;
	OBSSignal videoremovedSignal;
	OBSSignal videorenamedSignal;
	OBSSignal videoupdatePropertiesSignal;
	OBSData videooldSettings;
	OBSPropertiesView *videoview;
    //audio
    OBSSource audiosource;
	OBSSignal audioremovedSignal;
	OBSSignal audiorenamedSignal;
	OBSSignal audioupdatePropertiesSignal;
	OBSData audiooldSettings;
	OBSPropertiesView *audioview;
};

#endif   /* ----- #ifndef RECORDDIALOG_INC  ----- */

