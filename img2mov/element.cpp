#include "element.h"
#include "image.h"
#include <QPushButton>
#include <QLabel>

Element::Element()
  : m_elementLayout(new QVBoxLayout(this))
{
    m_elementLayout->setSpacing(0);
    m_elementLayout->setMargin(0);
    m_elementLayout->addWidget(new Image(tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    m_elementLayout->addWidget(new QPushButton(tr("Longer")));
    m_elementLayout->addWidget(new QLabel(tr("input text")));
    setLayout(m_elementLayout);
}
