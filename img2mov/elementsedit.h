#ifndef ELEMENTSEDIT_H
#define ELEMENTSEDIT_H

#include <QWidget>
#include <QScrollArea>
#include "flowlayout.h"
#include "element.h"
#include <QBuffer>
#include <QToolButton>
#include "comm.h"
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class GraphicsScene;
//! [0]
class ElementsEdit : public QWidget
//class ElementsEdit : public QScrollArea
{
    Q_OBJECT

public:
    ElementsEdit(QWidget *parent, GlobalContext* globalContext);
    ~ElementsEdit();
    Element* currentElement();
private:
    int callFfmpeg(const QVector<QString>& vqsArgv);
    void createFinalVideo(bool bPlay);
    void createFinalVideoMusicTxt(bool bPlay);
    void scaleImage(Element *element);
    void createSingleVideo(int idxElement);
    void createSimpleVideo(Element *element);
    void createPanzoomVideo(Element *element, int framerate, const QString& duration, const QString& panzoom);
    /*bool createAnimation(Element *firstElement, Element *secondElement
            , const QString& animationName);*/
    bool createAnimation(Element *firstElement, Element *secondElement, bool isFromPanzoom);
    bool createAnimationPanzoom(Element *firstElement, Element *secondElement);
    void initialProgress();
    void assignProgress();
    void updateTextAttrAndAss(int iStartIdx);
signals:
    void readyVideo(const QString& fileName, const QByteArray& buffer, int position);
    void changePlayPosition(int);
    void playVideo();
    //void createVideoTextSignal(void*);
    void createVideoTextSignal(Element*);
    void displayVideoTextSignal(Element*, bool);
    void activeVideoTextSignal(Element*, const QString&);
    void activeTabTextSignal(void*);
    void activeTabMusicSignal(GlobalMusicAttr*);
    void assignTabValueSignal();
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
public slots:
    void addImages();
    void removeImage();
    void addMusic();
    void addText();
    void selectedImage(QWidget*);
    void selectedText(const QString&);
    void selectedMusic();
    void selectedTransition(const QString& animation);
    void elementAttrChanged(bool bPlay);
    void durationChanged(qint64 duration);
    void audioDurationChanged(qint64 duration);
    void positionChanged(qint64 position);
    //void selectedText(void*);
    //void handleContextMenuRequested(const QPoint &pos);
    void updatedText(const QString&);
    void videoStateChanged(QMediaPlayer::State);
private:
    FlowLayout *m_flowLayout;
    QVBoxLayout *m_firstLayout;
    QLabel* m_firstLabel;

    //进度条竖线
    QFrame *m_vecticalLine; 
    qint64 m_duration;
    qint64 m_imgWidth;
    qint64 m_signalImgWidth;
    qint64 m_imgHeight;
    qint64 m_lineWidth;
    qint64 m_playPosition;
    //end

    qint64 m_imgPlayPosition; //选中图片计算出来的位移

    QString m_tmpdir;
    QWidget *m_lastSelectedElement;
    //QWidget *m_lastSelectedText;
    int m_idxCurrentElement;
    QByteArray m_playData;
    QBuffer m_playBuffer;

    bool m_isFirstClick;

    uint8_t* m_pOutBuffer;
    size_t m_outlen;
    size_t m_outMaxLen;
    uint8_t* m_pTextVideoOutBuffer; // 包含文字的视频
    size_t m_textVideoOutLen;
    size_t m_textVideoMaxOutLen;

    GlobalContext* m_globalContext;

    QByteArray m_qsInText;


    //audio
    QMediaPlayer m_audioMediaPlayer;
    GlobalMusicAttr* m_globalMusicAttr;
    bool m_isFirstMusic;
    //QString m_qsAudioFilename;
    //int m_iAudioStart;
    //int m_iAudioDuration;
    
    //video
};
//! [0]

#endif // ELEMENTSEDIT_H
