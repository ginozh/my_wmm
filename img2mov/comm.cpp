#include "comm.h"
#include <QDesktopWidget>
#include <QApplication>

GlobalTextAttr::GlobalTextAttr()
{
    //m_qfont = ;
    m_fontSize = "8";
    m_isBoldChecked = false;
    m_isItalicChecked = false;
    m_isUnderlineChecked = false;
    m_fontColor = Qt::white;

    m_textAlign = Qt::AlignLeft; //Qt::AlignHCenter; //AlignRight;
    m_idxEffects = 0;
    //m_qsStartTimeText = "0.00s";
    //m_qsDurationText = "2.00s";
    m_iStartTimeText = 0;
    m_iDurationText = 0;
}

GlobalAnimationAttr::GlobalAnimationAttr()
{
    //m_qsPanZoom="zoompan=z='zoom+0.001':s=512x384";
    m_iTransitionDuration = 1500;
}
GlobalVideoAttr::GlobalVideoAttr()
{
    m_iFramerate = 24;
    m_iDuration = 2000;
}
GlobalMusicAttr::GlobalMusicAttr()
{
    m_iStartTime = 0;
    m_iStartPoint = 0;
    m_iEndPoint = 0;
    m_iMusicDuration = 0;
    //m_iEntPoint = 2000;
}
GlobalContext* GlobalContext::m_pInstance = NULL;
GlobalContext::GlobalContext()
{
    //m_pInstance = NULL;

    m_elementsEdit=NULL;
    m_scene=NULL;
    m_player=NULL;
    m_tabWidget=NULL;

    QDesktopWidget* desktopWidget = QApplication::desktop();
    m_rectDesktop = desktopWidget->availableGeometry();
    m_dFactorX = (double)m_rectDesktop.width()/2452;
    m_dFactorY = (double)m_rectDesktop.height()/1444;
}
GlobalContext* GlobalContext::instance()  
{  
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new GlobalContext();  
    return m_pInstance;  
}  
#if 0
void GlobalContext::resetGeometry(QWidget *widget)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect rectDesktop = desktopWidget->availableGeometry();
    double factorx = rectDesktop.width()/2452;
    double factory = rectDesktop.height()/1444;    

    int widgetX = widget->x();
    int widgetY = widget->y();
    int widgetWid = widget->width();
    int widgetHei = widget->height();
    int nWidgetX = (int)(widgetX*factorx);
    int nWidgetY = (int)(widgetY*factory);
    int nWidgetWid = (int)(widgetWid*factorx);
    int nWidgetHei = (int)(widgetHei*factory);
    widget->setGeometry(nWidgetX,nWidgetY,nWidgetWid,nWidgetHei);
}
#endif
