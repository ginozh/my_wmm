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

class RecordDialog : public QDialog {
	Q_OBJECT
public:
    RecordDialog(QWidget *parent);
    ~RecordDialog();
private:
	QPointer<OBSQTDisplay> preview;
	QSplitter *windowSplitter;
};

#endif   /* ----- #ifndef RECORDDIALOG_INC  ----- */

