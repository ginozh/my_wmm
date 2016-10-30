#ifndef COMM_H
#define COMM_H

#include<QFont>
#include<QString>
#include<QColor>

class ElementsEdit;
class GraphicsScene;
class VideoPlayer;
class TabWidget;

//elementsedit.cpp
typedef struct stTextAttr{
public:
    stTextAttr(const QString& qsStartTimeText="2.00s", const QString& qsDurationText="2.00")
    {
        initTextAttrVar(qsStartTimeText, qsDurationText);
    }
    void initTextAttrVar(const QString& qsStartTimeText, const QString& qsDurationText)
    {
        //m_qfont = ;
        m_fontSize = "8";
        m_isBoldChecked = false;
        m_isItalicChecked = false;
        m_isUnderlineChecked = false;
        m_fontColor = Qt::white;

        m_textAlign = Qt::AlignHCenter; //AlignRight;
        m_idxEffects = 0;
        m_qsStartTimeText = qsStartTimeText;
        m_qsDurationText = qsDurationText;
    }
public:
    //QString m_qsText;
    //font
    QFont m_qfont;
    QString m_fontSize;
    bool m_isBoldChecked; //setChecked
    bool m_isItalicChecked;
    bool m_isUnderlineChecked;
    QColor m_fontColor;
    //Paragraph
    //Qt::Alignment m_textAlign; 
    int m_textAlign; 
    //Adjust
    QString m_qsStartTimeText;
    QString m_qsDurationText;
    //Effects
    int m_idxEffects;
    //ass
    QString m_qsStyle;
    QString m_qsEvent;

}stTextAttr;

//new in element.cpp
class GlobalAnimationAttr
{
public:
    GlobalAnimationAttr()
    {
        m_iTransitionDuration = 1500; //毫秒
        m_qsPanZoom = "zoompan=z='zoom+0.001':s=512x384"; //for test
    }
    ~GlobalAnimationAttr(){}
public:
    QString m_qsTransitionName;
    int m_iTransitionDuration;
    QString m_qsPanZoom;
};

//new in element.cpp
class GlobalVideoAttr
{
public:
    GlobalVideoAttr()
    {
        //毫秒
        m_iDuration = 2000;
        m_iFramerate = 24;
    }
    ~GlobalVideoAttr(){}
public:
    int m_iDuration;
    int m_iFramerate;
};

//new in elementsedit.cpp
class GlobalMusicAttr
{
public:
    GlobalMusicAttr()
    {
        //毫秒
        m_iStartTime = 0;//1200; 
        m_iStartPoint = 0;
        m_iEntPoint = 0;
        m_iAudioDuration = 0;
    }
    ~GlobalMusicAttr()
    {
    }
public:
    QString m_qsAudioFilename;
    int m_iStartTime;
    int m_iStartPoint;
    int m_iEntPoint;
    int m_iAudioDuration;
};


//new in mainwindow.cpp
class GlobalContext
{
public:
    GlobalContext(){
        m_elementsEdit=NULL;
        m_scene=NULL;
        m_player=NULL;
        m_tabWidget=NULL;
    }
    ~GlobalContext(){}
public:
    ElementsEdit* m_elementsEdit;
    GraphicsScene* m_scene;
    VideoPlayer* m_player;
    TabWidget* m_tabWidget;
};

#endif
