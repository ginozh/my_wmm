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
#include <memory>
#include "GLWidget.h"
#include "GLHiddenWidget.h"
#include "PlayerPrivate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
	GLWidget *m_mainView;
	//GLHiddenWidget *m_hiddenGl;
	GLWidget *m_hiddenGl;
    PlayerPrivate* m_playerprivate=NULL;

    int glw=1280;
    int glh=720;
};

#endif
