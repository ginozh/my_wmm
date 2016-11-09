#include "comm.h"

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

GlobalContext::GlobalContext()
{
    m_elementsEdit=NULL;
    m_scene=NULL;
    m_player=NULL;
    m_tabWidget=NULL;
}
