#include <QHBoxLayout>
#include <QScrollArea>
#include <QPalette>
#include "themes.h"
#include "videoplayer.h"

Themes::Themes(QWidget *parent)
    : QWidget(parent)
    , m_themeslist(new ThemesList)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    setLayout(hbox);
    {
        QScrollArea* m_scrollArea = new QScrollArea;
        hbox->addWidget(m_scrollArea);

        m_scrollArea->setBackgroundRole(QPalette::Light);
        m_scrollArea->setWidgetResizable (true);
        m_scrollArea->setWidget(m_themeslist);
    }
    {
        VideoPlayer* m_player = new VideoPlayer(this);
        hbox->addWidget(m_player);
    }
}
Themes::~Themes()
{
}
