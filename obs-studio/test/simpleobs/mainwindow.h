#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QApplication>
#include <QPointer>
#include "window-main.hpp"
//class RecordDialog;
//class OBSBasic;
//class OBSMainWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow(){};
public slots:
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
public:
    // OBSBasicPreview *preview;
    // RecordDialog* recordDialog=nullptr;
	QPointer<OBSMainWindow> mainDialog;

private:
};

#endif
