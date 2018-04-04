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
#include "GLDisplayWidget.h"
#include "GLHiddenWidget.h"
//#include "PlayerPrivate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
	GLDisplayWidget *m_playerWidget=NULL;
	GLHiddenWidget *m_hiddenWidget=NULL;
	//GLWidget *m_hiddenWidget;
    //PlayerPrivate* m_playerprivate=NULL;

    int glw=1280;
    int glh=720;

};

#endif
