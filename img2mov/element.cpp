#include "element.h"
#include <QPushButton>
#include <QLabel>

Element::Element(QWidget *parent, const QString& qsImage)
    : QWidget(parent)
    , m_elementLayout(new QVBoxLayout(this))
    , m_qsImageName(qsImage)
    , m_pimage(0)
{
    m_elementLayout->setSpacing(0);
    m_elementLayout->setMargin(0);
    //m_elementLayout->addWidget(new Image(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    if(!qsImage.isEmpty())
    {
        m_pimage = new Image(this, qsImage);
        //webenginewidgets/simplebrowser/tabwidget.cpp
        m_elementLayout->addWidget(m_pimage);
        connect(this, SIGNAL(insertImage()), parentWidget(), SLOT(load()) );
        connect(this, SIGNAL(selectedImageSignal()), parentWidget(), SLOT(selectedImage()) );
#if 0
        m_pimage->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_pimage, SIGNAL(customContextMenuRequested(QPoint)), parentWidget(), SLOT(handleContextMenuRequested(QPoint)) );
#endif
        //connect(this, &QLabel::customContextMenuRequested, parent()->parent(), &ElementsEdit::handleContextMenuRequested);

    }
    m_elementLayout->addWidget(new QPushButton(tr("")));
    m_elementLayout->addWidget(new QLabel(tr("input text")));
    setLayout(m_elementLayout);
}
void Element::insert()
{
    emit insertImage();
}
void Element::selectedImage()
{
    emit selectedImageSignal();
}
