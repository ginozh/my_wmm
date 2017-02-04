#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>
#include "preview.h"
#include "videoplayer.h"
#include "comm.h"

Preview::Preview(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout;
    setLayout(hbox);

    QVBoxLayout* vboxTitle = new QVBoxLayout;
    hbox->addLayout(vboxTitle);
    {
        QLabel* lbl = new QLabel(tr("OPENING TITLE"));
        vboxTitle->addWidget(lbl);
    }
    {
        QPlainTextEdit* pte = new QPlainTextEdit;
        vboxTitle->addWidget(pte);
        GlobalContext* globalContext = GlobalContext::instance();

        QFont fontDeviation;
        fontDeviation.setPointSize(1);
        QFontMetrics fmDeviation(fontDeviation);
        pte->setMaximumHeight(30*globalContext->m_dFactorX*fmDeviation.height());
    }
    {
        QLabel* lbl = new QLabel(tr("CLOSING TITLE"));
        vboxTitle->addWidget(lbl);
    }
    {
        QPlainTextEdit* pte = new QPlainTextEdit;
        vboxTitle->addWidget(pte);

        GlobalContext* globalContext = GlobalContext::instance();

        QFont fontDeviation;
        fontDeviation.setPointSize(1);
        QFontMetrics fmDeviation(fontDeviation);
        pte->setMaximumHeight(30*globalContext->m_dFactorX*fmDeviation.height());
    }
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        vboxTitle->addWidget(nullWidget);
    }
    {
        QCheckBox* m_useStylePaletteCheckBox = new QCheckBox(tr("KEEP ORIGINAL SOUND"),this);
        vboxTitle->addWidget(m_useStylePaletteCheckBox);
    }
#if 0
    {
        QWidget* nullWidget = new QWidget;
        nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        vboxTitle->addWidget(nullWidget);
    }
#endif

    QVBoxLayout* vboxPlay = new QVBoxLayout;
    hbox->addLayout(vboxPlay);
    {
        VideoPlayer* m_player = new VideoPlayer(this);
        vboxPlay->addWidget(m_player);
    }
}

Preview::~Preview()
{
}
