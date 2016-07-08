#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "elementsedit.h"
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTreeView)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QTreeView *m_treeView;
    QTextEdit *m_detailsText;
    VideoPlayer *m_player;
    ElementsEdit *m_elementsEdit;
};

#endif // MAINWINDOW_H
