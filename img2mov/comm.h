#ifndef COMM_H
#define COMM_H

#include<QFont>
#include<QString>
#include<QColor>
#include<QRect>

class ElementsEdit;
class GraphicsScene;
class VideoPlayer;
class TabWidget;

enum ATTR_TYPE
{
    ATTR_NOTHING =0
    ,ATTR_VIDEO = 0x1
    ,ATTR_MUSIC = 0x2
    ,ATTR_TEXT =0x4
    ,ATTR_ROTATE =0x8
};

// image
class GlobalImageAttr
{
public:
    GlobalImageAttr();
    GlobalImageAttr(const GlobalImageAttr& globalImageAttr);
public:
    QString m_qsImageName; //图片路径
    QSize m_iSize; //原始尺寸
    QSize m_iScaledSize; //缩放尺寸
    int m_iRotateLeft;
    int m_iRotateRight;
};
//element
class GlobalTextAttr
{
public:
    GlobalTextAttr();
public:
    //QString m_qsText;
    //font
    QFont m_qfont;
    //QString m_fontSize;
    //bool m_isBoldChecked; //setChecked
    //bool m_isItalicChecked;
    //bool m_isUnderlineChecked;
    QColor m_fontColor;
    //Paragraph
    //Qt::Alignment m_textAlign; 
    int m_textAlign; 
    //Adjust
    //QString m_qsStartTimeText;
    //QString m_qsDurationText;
    int m_iStartTimeText ;
    int m_iDurationText ;
    //Effects
    int m_idxEffects;
    //
    QPointF m_pfPos;
    //ass
    QString m_qsStyle;
    QString m_qsEvent;

};
class GlobalAnimationAttr 
{
public:
    GlobalAnimationAttr();
public:
    QString m_qsTransitionName;
    QString m_qsPanZoom;
    int m_iTransitionDuration;
};
class GlobalVideoAttr 
{
public:
    GlobalVideoAttr();
public:
    int m_iFramerate;
    int m_iDuration;
};

// elemensedit
class GlobalMusicAttr
{
public:
    GlobalMusicAttr();
public:
    QString m_qsMusicFullFilename;
    QString m_qsMusicFilename;
    int m_iStartTime;
    int m_iStartPoint;
    int m_iEndPoint;
    int m_iMusicDuration;
};

// mainwindow
class GlobalContext
{
private:
    GlobalContext();
    static GlobalContext *m_pInstance;
public:
    static GlobalContext* instance();
public:
    ElementsEdit* m_elementsEdit;
    GraphicsScene* m_scene;
    VideoPlayer* m_player;
    TabWidget* m_tabWidget;
    QRect m_rectDesktop;
    double m_dFactorX;
    double m_dFactorY;
    QSize m_iScaledSize; //整体缩放尺寸,包括: 图片、scene、ass
    int m_iFramerate;
};

#endif
