#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QApplication>
#include <QAbstractButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QVector>
#include <QPainter>
#include <QFontComboBox>
#include <QSharedPointer>
#include <memory>
#include "ui/viewerwidget.h"
#include "panels/viewer.h"

class Project;
class EffectControls;
class Viewer;
class ViewerWidget;
class ViewerContainer;
class Timeline;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
	~MainWindow();

public slots:

protected:

private slots:
private:
    Viewer* m_Viewer;
    ViewerWidget* m_viewer_widget;
    ViewerContainer* viewer_container;
};
extern MainWindow* mainWindow;

#endif
