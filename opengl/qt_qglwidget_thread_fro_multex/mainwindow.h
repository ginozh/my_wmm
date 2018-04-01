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
	GLHiddenWidget *m_hiddenGl;
	GLWidget *m_mainView;
};

#endif
