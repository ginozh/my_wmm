#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QDebug>
#include "format.h"
#include "comm.h"

Format::Format(QWidget *parent)
    : QWidget(parent)
    , m_vecticalLine(new QFrame)
    , m_dlgSetting(new QDialog)
{
    m_dlgSetting->setModal(true);

    QHBoxLayout* hbox = new QHBoxLayout;
    setLayout(hbox);

    {
        QGridLayout *layout = new QGridLayout;
        hbox->addLayout(layout);
        for (int i = 0; i < 4; ++i) {
            QLabel* label1 = new QLabel(tr("Line %1.1:").arg(i + 1));
            QLabel* label2 = new QLabel(tr("Line %1:2").arg(i + 1));
            QLabel* label3 = new QLabel(tr("Line %1:3").arg(i + 1));
            layout->addWidget(label1, i, 0);
            layout->addWidget(label2, i, 1);
            layout->addWidget(label3, i, 2);
        }
    }
    {
        hbox->addWidget(m_vecticalLine);

        GlobalContext* m_globalContext = GlobalContext::instance();
        int m_lineWidth=10*m_globalContext->m_dFactorY;
        m_vecticalLine->setEnabled(true);
        //m_vecticalLine->setGeometry(QRect(300, 250, 10, 200));
        m_vecticalLine->setLineWidth(0);
        m_vecticalLine->setMidLineWidth(m_lineWidth);

        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        m_vecticalLine->setPalette(palette1);
        m_vecticalLine->setFrameShape(QFrame::VLine);
        m_vecticalLine->setFrameShadow(QFrame::Sunken);
        m_vecticalLine->show();
    }
    {
        QVBoxLayout* vbox = new QVBoxLayout(this);
        hbox->addLayout(vbox);
        {
            QLabel* lbl = new QLabel(tr("Output descriptions"));
            vbox->addWidget(lbl);

            QHBoxLayout* hboxName = new QHBoxLayout;
            vbox->addLayout(hboxName);
            {
                QLabel* lblName = new QLabel(tr("Name: "));
                hboxName->addWidget(lblName);
                QLineEdit* le = new QLineEdit(tr("My Video"));
                hboxName->addWidget(le);
            }
            QHBoxLayout* hboxSave = new QHBoxLayout;
            vbox->addLayout(hboxSave);
            {
                QLabel* lblName = new QLabel(tr("Save to: "));
                hboxSave->addWidget(lblName);
                QLineEdit* le = new QLineEdit;
                hboxSave->addWidget(le);
            }

            QHBoxLayout* hboxResolution = new QHBoxLayout;
            vbox->addLayout(hboxResolution);
            {
                QLabel* lblName = new QLabel(tr("Resolution: "));
                hboxResolution->addWidget(lblName);
                QLabel* lblValue = new QLabel(tr("640*360"));
                hboxResolution->addWidget(lblValue);

                QToolButton* m_boldButton = new QToolButton;
                m_boldButton->setText(tr("Settings"));
                hboxResolution->addWidget(m_boldButton);
                connect(m_boldButton, SIGNAL(clicked(bool)), 
                        this, SLOT(handleSetting())); 
            }
            
            QHBoxLayout* hboxSize = new QHBoxLayout;
            vbox->addLayout(hboxSize);
            {
                QLabel* lblName = new QLabel(tr("Size: "));
                hboxSize->addWidget(lblName);
                QLabel* lblValue = new QLabel(tr("4.7MB"));
                hboxSize->addWidget(lblValue);
            }

            QHBoxLayout* hboxDuration = new QHBoxLayout;
            vbox->addLayout(hboxDuration);
            {
                QLabel* lblName = new QLabel(tr("Duration: "));
                hboxDuration->addWidget(lblName);
                QLabel* lblValue = new QLabel(tr("00:00:23"));
                hboxDuration->addWidget(lblValue);
            }
        }
        {
            QWidget* nullWidget = new QWidget;
            nullWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            vbox->addWidget(nullWidget);
        }
    }
}

Format::~Format()
{
}
void Format::handleSetting()
{
    m_dlgSetting->show();
}
