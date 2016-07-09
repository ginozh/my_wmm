#include "element.h"
#include "image.h"
#include <QPushButton>
#include <QLabel>

Element::Element(QWidget *parent, const QString& qsImage)
    : QWidget(parent)
    , m_elementLayout(new QVBoxLayout(this))
{
    m_elementLayout->setSpacing(0);
    m_elementLayout->setMargin(0);
    //m_elementLayout->addWidget(new Image(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    if(!qsImage.isEmpty())
    {
        Image *pimage = new Image(this, qsImage);
        //webenginewidgets/simplebrowser/tabwidget.cpp
        m_elementLayout->addWidget(pimage);
#if 0
        pimage->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(pimage, SIGNAL(customContextMenuRequested(QPoint)), parentWidget(), SLOT(handleContextMenuRequested(QPoint)) );
#endif
        //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);

    }
    m_elementLayout->addWidget(new QPushButton(tr("")));
    m_elementLayout->addWidget(new QLabel(tr("input text")));
    setLayout(m_elementLayout);
}
