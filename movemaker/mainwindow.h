#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QToolButton>
#include "mediasedit.h"
#include "themes.h"
#include "musics.h"

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
    void createStackWidget(QVBoxLayout *mainLayout);
    void createPageUpDown(QVBoxLayout *mainLayout);
private:
    QVBoxLayout *mainLayout;
    QStackedLayout* m_stackedLayout;
    QWidget* qwidgetmedia;
    // MediasEdit* qwidgetmedia;
    MediasEdit* m_mediasEdit;
    Themes* m_themes;
    Musics* m_musics;


    QWidget* qwidgettheme;
    QWidget* qwidgetmusic;
    QWidget* qwidgetpreview;
    QWidget* qwidgetsave;

    QToolButton* m_mediaButton;
    QToolButton* m_themeButton;
    QToolButton* m_musicButton;
    QToolButton* m_previewButton;
    QToolButton* m_saveButton;

};

#endif // MAINWINDOW_H
