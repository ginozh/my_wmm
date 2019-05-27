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

class Project;
class OpenGLWidget;
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
    OpenGLWidget* m_viewer_widget=NULL;
};
extern MainWindow* mainWindow;

#endif
