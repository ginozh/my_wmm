#include <QPushButton>
#include <QPalette>
#include <QScrollBar>
#include <QFileDialog>
#include <QStandardPaths>

#include "elementsedit.h"
#include "element.h"
//! [1]
ElementsEdit::ElementsEdit(QWidget *parent)
    : QWidget(parent)
      , m_flowLayout(new FlowLayout(this))
{
    setBackgroundRole(QPalette::Dark);
#if 0
    m_flowLayout->addWidget(new QPushButton(tr("Short")));
    m_flowLayout->addWidget(new QPushButton(tr("Longer")));
    m_flowLayout->addWidget(new QPushButton(tr("Different text")));
    m_flowLayout->addWidget(new QPushButton(tr("More text")));
    m_flowLayout->addWidget(new QPushButton(tr("Even longer button text")));
#endif
    //m_flowLayout->setSpacing(0);
    //m_flowLayout->setMargin(0);
    //m_flowLayout->setContentsMargins(0,0,0,0);
#if 1
    /*
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    m_flowLayout->addWidget(new Element());
    */
    //test
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img001.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img002.jpg")));
    m_flowLayout->addWidget(new Element(this, tr("C:\\QtProjects\\qtmovie\\jpg\\img003.jpg")));
    setLayout(m_flowLayout);
#else
    QVBoxLayout *layout    = new QVBoxLayout(this);
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    layout->addWidget(new Element());
    setLayout(layout);
#endif


#if 0
    setVisible(true);
    //adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    QScrollBar *scrollBar = horizontalScrollBar();
    double factor = 1.0;
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
#endif 
    setWindowTitle(tr("Flow Layout"));
}
//! [1]
void ElementsEdit::load()
{
    //qtconcurrent/imagescaling
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Images"),
            /*QDir::currentPath()*/QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            "*.jpg *.png");
    if (files.count() == 0)
        return;
    for (int i = 0; i < files.count(); ++i) {
        //m_flowLayout->addWidget(new Element(this, files[i]));
        m_flowLayout->insertWidget(0, new Element(this, files[i]));
        //m_flowLayout->addWidget(new Element(this, files[i]), 1, 1);
    }

    /*if (!m_canvas->loadImage(fileName))
        QMessageBox::information(this, "Error Opening Picture",
                                 "Could not open picture");
                                 */
}
void ElementsEdit::handleContextMenuRequested(const QPoint &pos)
{
    load();
}
