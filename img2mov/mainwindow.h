#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoplayer.h"
#include "elementsedit.h"
#include "menu.h"
#include "tabwidget.h"
#include "comm.h"
#ifdef TEST_QSS
#include <QCheckBox>
#endif

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
    GlobalContext* globalContext(){return m_globalContext;}
#if 0
private slots:
    void addImages();
#endif
protected:
    //void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
public slots:
#ifdef TEST_ZOOMPAN
    void assignZoomPan();
#endif
#ifdef TEST_QSS
    void changeStyle(const QString &styleName);
    void changePalette();
#endif
private:
    QWidget *m_centralWidget;
    //TabWidget *m_tabWidget;
    Menu *m_menu;

    QTreeView *m_treeView;
    QTextEdit *m_detailsText;
    //VideoPlayer *m_player;
    //ElementsEdit *m_elementsEdit;
    QScrollArea *m_scrollArea;

    GlobalContext* m_globalContext;
#ifdef TEST_ZOOMPAN
    QLineEdit* m_leZoomPan;
    QToolButton *m_tbZoomPan;
#endif
#ifdef TEST_QSS
    QPalette m_originalPalette;
    QComboBox *m_styleComboBox;
    QCheckBox *m_useStylePaletteCheckBox;
#endif
};

#endif // MAINWINDOW_H
