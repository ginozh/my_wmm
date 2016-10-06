#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "elementsedit.h"
#include "menu.h"
#include "tabwidget.h"
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QTreeView)

QT_BEGIN_NAMESPACE
class QScrollArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
#if 0
private slots:
    void load();
#endif
protected:
    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
private:
    QWidget *m_centralWidget;
    TabWidget *m_tabWidget;
    Menu *m_menu;

    QTreeView *m_treeView;
    QTextEdit *m_detailsText;
    VideoPlayer *m_player;
    ElementsEdit *m_elementsEdit;
    QScrollArea *m_scrollArea;
};

#endif // MAINWINDOW_H
