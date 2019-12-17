#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "glwidget.h"
#include "fileprocessor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void onAddNew();
private:
    GLWidget* glWidget;
    QLineEdit* m_leUrl;
    QPushButton* m_pbOpenFile;
    QPushButton* m_pbStart;
    FileProcessor* m_fileProcessor;
    QString m_url;
};

#endif
