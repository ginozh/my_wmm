#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "mainwindow.h"
#include <QFrame>
#include <QFont>
#include <QToolButton>
#include <QMenu>
#include <QTest> 
#include <QTabBar> 
#include <QComboBox> 
#include <QDoubleValidator> 
#include <QDebug> 
void TabWidget::boxValueChanged(double v)
{
    //QMessageBox::information(NULL, "info", QString(tr("v: %1")).arg(v));
    qDebug()<<"v: "<<v;
}
TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    //tabBar()->setExpanding(false);
    int iHeight = 230;
    int m_iCellHeight = iHeight/4;
    QSize m_iconSize(150,110);
    QWidget* m_tabHome = new QWidget(this);
    addTab(m_tabHome, tr("&Home"));

    QHBoxLayout *hbox = new QHBoxLayout;
    m_tabHome->setLayout(hbox);
    {
        QVBoxLayout *vboxAdd = new QVBoxLayout;
        vboxAdd->setGeometry(QRect(0, 0, 3*m_iCellHeight, 4*m_iCellHeight));
        hbox->addLayout(vboxAdd);
        {
            QHBoxLayout* hboxDuration = new QHBoxLayout;
            vboxAdd->addLayout(hboxDuration);
            {
                QLabel *lbl = new QLabel(tr("Duration(s):"));

                hboxDuration->addWidget(lbl);
            }
            {
                SpinBox* m_cbDurationVideo = new SpinBox();

                m_cbDurationVideo->setRange(1, 23);
                hboxDuration->addWidget(m_cbDurationVideo);
                connect(m_cbDurationVideo, SIGNAL(textChangedSignal(double)),this, SLOT(boxValueChanged(double)));
            }
            {
                QComboBox* m_cbDurationVideo = new QComboBox();
                m_cbDurationVideo->setEditable(true);
                m_cbDurationVideo->setCurrentText(QString(tr("2.00")));
                m_cbDurationVideo->addItem(QString(tr("2.00")));
                QDoubleValidator* validator = new QDoubleValidator();
                validator->setRange(0.5, 5.0, 2);
                validator->setNotation(QDoubleValidator::StandardNotation);
                //m_cbDurationVideo->setValidator(validator); //uncomplete delete old
                m_cbDurationVideo->setValidator(new lvalidator); //uncomplete delete old

                hboxDuration->addWidget(m_cbDurationVideo);
            }
        }
        {
            QLabel *lbl = new QLabel(tr("Add"));
            lbl->setAlignment(Qt::AlignCenter);
            lbl->setFixedWidth(3*m_iCellHeight);

            vboxAdd->addWidget(lbl);
        }
    }
    {
        appendLine(hbox);
    }
    QWidget *tmp = new QWidget;
    tmp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    hbox->addWidget(tmp);
}
MainWindow::MainWindow()
{
    setWindowTitle(tr("progress"));
    TabWidget* m_tabWidget = new TabWidget();

    QWidget* m_centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    m_centralWidget->setLayout(mainLayout);

    setCentralWidget(m_centralWidget);

    qApp->setStyleSheet("QTabWidget::tab-bar { alignment: left; }");
}
void TabWidget::appendLine(QHBoxLayout *hbox)
{
    QFrame *line = new QFrame();
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hbox->addWidget(line);
}
