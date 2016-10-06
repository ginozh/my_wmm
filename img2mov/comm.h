#ifndef COMM_H
#define COMM_H

#include<QFont>
#include<QString>
#include<QColor>

class ElementsEdit;
class DiagramScene;
class VideoPlayer;
class TabWidget;

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
    Qt::Alignment m_textAlign; 
    //Adjust
    QString m_qsStartTimeText;
    QString m_qsDurationText;
    //Effects
    int m_idxEffects;

}stTextAttr;

typedef struct stContext{
}stContext;

#endif
