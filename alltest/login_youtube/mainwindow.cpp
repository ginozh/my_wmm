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
TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
{
    m_pOAuth2 = new OAuth2(this);
    m_pOAuth2->setScope("https://www.googleapis.com/auth/books");
    m_pOAuth2->setClientID("578932727592-rdfo83pcnv3l1ccges9qi6trs72aonfk.apps.googleusercontent.com");
    m_pOAuth2->setRedirectURI("http://localhost:8080");
    m_pOAuth2->setCompanyName("filmmaker");
    m_pOAuth2->setAppName("test");

    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(loginDone()));
    
}
void TabWidget::startLogin(bool bForce) 
{ 
    m_pOAuth2->startLogin(bForce); 
}
void TabWidget::loginDone()
{
    qDebug()<<"token: "<<m_pOAuth2->accessToken();
}
QString TabWidget::token()
{
    return m_pOAuth2->accessToken();
}
MainWindow::MainWindow()
{
    setWindowTitle(tr("progress"));
    TabWidget* m_tabWidget = new TabWidget();

    QWidget* m_centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    lblDurationTrans = new QLabel(QString(tr("token: --")));
    mainLayout->addWidget(lblDurationTrans);
    m_centralWidget->setLayout(mainLayout);

    setCentralWidget(m_centralWidget);

    m_tabWidget->startLogin(true);
    lblDurationTrans->setText(QString(tr("token: -%1-")).arg(m_tabWidget->token()));
}
