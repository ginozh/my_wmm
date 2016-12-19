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
    //m_qsPanZoom="zoompan=z='zoom+0.001':x='if(gte(zoom,1.5),x,x+1)':y='y':s=512x384";
    //m_qsPanZoom="scale=8000:-1,zoompan=z='zoom+0.001':x='if(gte(zoom,1.5),x,x+1)':y='y':s=512x384";
    m_iTransitionDuration = 1500;
}
GlobalVideoAttr::GlobalVideoAttr()
{
    m_iFramerate = GlobalContext::instance()->m_iFramerate;
    m_iDuration = 2000;
}
void GlobalMusicAttr::initialMusicAttr()
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

    //m_qsPanZoom="zoompan=z='zoom+0.001':x='if(gte(zoom,1.5),x,x+1)':y='y':s=512x384";
    
    m_mapPanZoom["none"]="";
    m_mapPanZoom["panupleft"]="zoompan=z='1.2':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'";
    m_mapPanZoom["panup"]="zoompan=z='1.2':x='(zoom-1)*iw/2':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'";
    m_mapPanZoom["panupright"]="zoompan=z='1.2':x='(zoom-1)*iw':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'";
    m_mapPanZoom["pandownleft"]="zoompan=z='1.2':y='min(y+(zoom-1)*ih/2/25,(zoom-1)*ih/2)'";
    m_mapPanZoom["pandown"]="zoompan=z='1.2':x='(zoom-1)*iw/2':y='min(y+(zoom-1)*ih/2/25,(zoom-1)*ih/2)'";
    m_mapPanZoom["pandownright"]= "zoompan=z='1.2':x='(zoom-1)*iw':y='min(y+(zoom-1)*ih/2/25,(zoom-1)*ih/2)'";
    m_mapPanZoom["panrighttop"]="zoompan=z='1.2':x='min(x+(zoom-1)*iw/2/25,(zoom-1)*iw/2)'";
    m_mapPanZoom["panright"]="";
    m_mapPanZoom["panrightbottom"]="";
    m_mapPanZoom["panlefttop"]="";
    m_mapPanZoom["panleft"]="";
    m_mapPanZoom["panleftbottom"];
    m_mapPanZoom["zoomincenter"]="zoompan=z='zoom+0.01':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2'";
    //放大一分钟(zoom:1.25)，旋转一分钟(t:1)
    //m_mapPanZoom["zoomincenterrotateright"]="zoompan=z='min(zoom+0.01,1.25)':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=512x384,rotate='if(gte(t,1), 0, 2*PI*t)'"; //uncomple: 512*384
    m_mapPanZoom["zoomincenterrotateright"]="zoompan=z='min(zoom+0.01,1.25)':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=M_SCALE,rotate='if(gte(t,1), 0, 2*PI*t)'"; //uncomple: 512*384
    //m_mapPanZoom["zoomincenterrotateleft"]="zoompan=z='min(zoom+0.01,1.25)':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=512x384,rotate='if(gte(t,1), 0, -2*PI*t)'"; //uncomple: 512*384
    m_mapPanZoom["zoomincenterrotateleft"]="zoompan=z='min(zoom+0.01,1.25)':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=M_SCALE,rotate='if(gte(t,1), 0, -2*PI*t)'"; //uncomple: 512*384
    m_mapPanZoom["zoomintopleft"]="zoompan=z='min(zoom+0.001,1.2)'";
    m_mapPanZoom["zoomintop"]="zoompan=z='zoom+0.01':mwhx='1':x='(iw-mw)/2'";
    m_mapPanZoom["zoomintopright"]="zoompan=z='zoom+0.01':mwx='1':x='iw-mw'";
    m_mapPanZoom["zoominright"]="zoompan=z='zoom+0.01':mwx='1':x='iw-mw':mhhy='1':y='(ih-mh)/2";
    m_mapPanZoom["zoominbottomright"]="zoompan=z='zoom+0.01':mwx='1':x='iw-mw':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoominbottom"]="zoompan=z='zoom+0.01':mwhx='1':x='(iw-mw)/2':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoominbottomleft"]="zoompan=z='zoom+0.01':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoominleft"]="zoompan=z='zoom+0.01':mhhy='1':y='(ih-mh)/2'";
    m_mapPanZoom["zoomoutcenter"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2'";  // 必须是:0.01或者0.005(有点抖动)
    m_mapPanZoom["zoomouttopleft"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))'";
    m_mapPanZoom["zoomouttop"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2'";
    m_mapPanZoom["zoomouttopright"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwx='1':x='iw-mw'";
    m_mapPanZoom["zoomoutright"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwx='1':x='iw-mw':mhhy='1':y='(ih-mh)/2";
    m_mapPanZoom["zoomoutbottomright"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwx='1':x='iw-mw':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoomoutbottom"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoomoutbottomleft"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mhy='1':y='ih-mh'";
    m_mapPanZoom["zoomoutleft"]="zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mhhy='1':y='(ih-mh)/2'";
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
