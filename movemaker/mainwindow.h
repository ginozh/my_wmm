#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QToolButton>
#include "mediasedit.h"
#include "themes.h"
#include "musics.h"
#include "preview.h"
#include "save.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
public slots:
    void handleTabChange();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    void createMenu(QVBoxLayout *mainLayout);
    void createTabBar(QVBoxLayout *mainLayout);
    void createStackWidget();
    void createPageUpDown(QVBoxLayout *mainLayout);
private:
    // QVBoxLayout *mainLayout;
    QStackedLayout* m_stackedLayout;
    // MediasEdit* m_qwidgetmedia;
    MediasEdit* m_mediasEdit;
    Themes* m_themes;
    Musics* m_musics;
    Preview* m_preview;


    QWidget* m_qwidgetmedia;
    QWidget* m_qwidgettheme;
    QWidget* m_qwidgetmusic;
    QWidget* m_qwidgetpreview;
    Save* m_save;

    QToolButton* m_mediaButton;
    QToolButton* m_themeButton;
    QToolButton* m_musicButton;
    QToolButton* m_previewButton;
    QToolButton* m_saveButton;

};

#endif // MAINWINDOW_H
