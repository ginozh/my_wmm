#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QToolButton>
#include <QDebug>
#include "save.h"

Save::Save(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    setLayout(hbox);

    QVBoxLayout* vboxtab = new QVBoxLayout;
    hbox->addLayout(vboxtab);
#define NEW_TAB(var_name, var_text) do \
    { \
        var_name = new QToolButton(this); \
        var_name->setStyleSheet("border-style: flat;"); \
        var_name->setText(var_text); \
        connect(var_name, SIGNAL(clicked(bool)), \
                this, SLOT(handleTabChange())); \
        vboxtab->addWidget(var_name); \
    }while(0)

    NEW_TAB(m_btnFormat, "Format");
    NEW_TAB(m_btnDevice, "Device");
    NEW_TAB(m_btnYoutube, "YouTube");
    NEW_TAB(m_btnFacebook, "Facebook");
    NEW_TAB(m_btnVimeo, "Vimeo");
#undef NEW_TAB

    m_stackedLayout = new QStackedLayout;
    hbox->addLayout(m_stackedLayout);
}

Save::~Save()
{
}

void Save::handleTabChange()
{
    QToolButton* qwidget = qobject_cast<QToolButton *>(sender());
    qDebug()<< "Save::handleTabChange. qwidget: " << qwidget;
    // qwidget->raise();
    if(m_btnFormat == qwidget)
        m_stackedLayout->setCurrentWidget(m_format);
#if 0
    else if(m_btnDevice == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgettheme);
    else if(m_btnYoutube == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetmusic);
    else if(m_btnFacebook == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetpreview);
    else if(m_btnVimeo == qwidget)
        m_stackedLayout->setCurrentWidget(qwidgetsave);
#endif
}
void Save::showEvent(QShowEvent *event)
{
    createStackWidget();
    QWidget::showEvent(event);
}
void Save::createStackWidget()
{
    m_format = new Format;
    m_stackedLayout->addWidget(m_format);
}
