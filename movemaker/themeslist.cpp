#include <QToolButton>
#include "themeslist.h"

ThemesList::ThemesList(QWidget *parent)
    : QWidget(parent)
{
    m_flowLayout = new FlowLayout(this);
    setLayout(m_flowLayout);

    QToolButton* m_mediaButton = new QToolButton(this);
    m_mediaButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_mediaButton->setIcon(QIcon("images/Simple.png"));
    m_mediaButton->setText("Simple");
    m_mediaButton->setStyleSheet("border-style: flat;");
    m_flowLayout->insertWidget(m_flowLayout->count(), m_mediaButton);
}
ThemesList::~ThemesList()
{
}
