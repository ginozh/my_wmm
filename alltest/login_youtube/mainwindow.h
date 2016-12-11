#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include "oauth2.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent=NULL);
    void startLogin(bool bForce) ;
    QString token();
public slots:
    void loginDone();
private:
    OAuth2* m_pOAuth2;
};

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
private:
    QLabel *lblDurationTrans;
};
//! [0]

#endif
