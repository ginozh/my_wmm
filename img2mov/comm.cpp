#include "comm.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QFileInfo> 

GlobalImageAttr::GlobalImageAttr()
{
    m_iScaledSize = GlobalContext::instance()->m_iScaledSize;

    m_iRotateLeft = 0;
    m_iRotateRight = 0;
}
GlobalImageAttr::GlobalImageAttr(const GlobalImageAttr& globalImageAttr)
{
    m_qsImageName = globalImageAttr.m_qsImageName;
    m_iSize = globalImageAttr.m_iSize;
    m_iScaledSize = globalImageAttr.m_iScaledSize;
    m_iRotateLeft = globalImageAttr.m_iRotateLeft;
    m_iRotateRight = globalImageAttr.m_iRotateRight;
}
GlobalTextAttr::GlobalTextAttr()
    :m_pfPos(-1, -1)
{
    //m_qfont = ;
    //m_fontSize = "8";
    //m_isBoldChecked = false;
    //m_isItalicChecked = false;
    //m_isUnderlineChecked = false;
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
    m_iFramerate = GlobalContext::instance()->m_iFramerate;
    m_iDuration = 2000;
}
GlobalMusicAttr::initialMusicAttr()
{
    m_qsMusicFullFilename = "";
    m_qsMusicFilename = "";
    m_iStartTime = 0;
    m_iStartPoint = 0;
    m_iEndPoint = 0;
    m_iMusicDuration = 0;
}
const QString& GlobalMusicAttr::musicFileName()
{
    if(!m_qsMusicFullFilename.isEmpty() && m_qsMusicFilename.isEmpty())
    {
        QFileInfo fi(m_qsMusicFullFilename);
        m_qsMusicFilename = fi.baseName();
    }
    return m_qsMusicFilename;
}
GlobalMusicAttr::GlobalMusicAttr()
{
    initialMusicAttr();
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
    QRect rectDesktop = desktopWidget->availableGeometry();
    m_dFactorX = (double)rectDesktop.width()/2452;
    m_dFactorY = m_dFactorX;//(double)rectDesktop.height()/1444;
    int iTmpW=((int)(m_dFactorX*512/8))*8; // 8而不是4，因为高度需要2的倍数
    int iTmpH=(iTmpW/4)*3;
    qDebug()<<"m_iScaledSize. m_dFactorX: "<<m_dFactorX<<" m_dFactorY: "<<m_dFactorY<<" w: "<<iTmpW<<" h:"<<iTmpH;
    m_iScaledSize = QSize(iTmpW, iTmpH);//QSize(m_dFactorX*512, m_dFactorX*384); //4:3

    m_iFramerate = 25;

    m_iFirstTabSize = QSize((int)(100*m_dFactorX),(int)(40*m_dFactorY) );
    m_iOtherTabSize = QSize((int)(180*m_dFactorX), m_iFirstTabSize.height() );
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
