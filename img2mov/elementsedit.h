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
#include <QMessageBox>

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
    ElementsEdit(QWidget *parent);
    ~ElementsEdit();
    Element* currentElement();
    GlobalMusicAttr* globalMusicAttr(){return m_globalMusicAttr;}
    qint64 totalVideoDuration(){return m_iTotalVideoDuration;}
    QMessageBox::StandardButton confirmSaveProject();
private:
    int callFfmpeg(const QVector<QString>& vqsArgv);
    void createFinalVideo(bool bPlay, QByteArray qbAss="", const QString& qsFinalVideoFile="", bool bCreateVideoFile=false);
    //void createFinalVideoMusicTxt(bool bPlay);
    //void scaleImage(Element *element);
    void scaleImage(Element *element, QSize qScaleSize=QSize(0, 0), bool bCreateVideoFile=false);
    //void createSimpleVideo(Element *element, bool bUseScaledImage=true);
    void createSimpleVideo(Element *element, bool bCreateVideoFile=false);
    //void createSingleVideo(int idxElement);
    void createSingleVideo(int idxElement, bool bCreateSimpleVideo=true, bool bCreateVideoFile=false);
    void createPanzoomVideo(Element *element, int framerate, const QString& duration, const QString& panzoom, bool bCreateVideoFile=false);
    /*bool createAnimation(Element *firstElement, Element *secondElement
            , const QString& animationName);*/
    bool createAnimation(Element *firstElement, Element *secondElement, bool isFromPanzoom, bool bCreateVideoFile=false);
    bool createAnimationPanzoom(Element *firstElement, Element *secondElement, bool bCreateVideoFile=false);
    void initialProgress();
    void assignProgress();
    void updateTextAttrAndAss(int iStartIdx);
    void initialFirstLayout();
    void musicAttrChanged();
    void createMusicArgs(QVector<QString>& vqsArgv, bool bCreateFinalVedeo);
    void createMusic();
    //Element* initialImage(const QString& fileName, int& idx);
    Element* initialImage(const GlobalImageAttr& newGlobalImageAttr, int& idx);
    void createFlowLayout();
signals:
    void readyVideo(const QString& fileName, const QByteArray& buffer, int position);
    void changePlayPosition(int);
    void updatedVideoTimeTextSignal(int, int);
    void playVideo();
    //void createVideoTextSignal(void*);
    void createVideoTextSignal(Element*);
    void displayVideoTextSignal(Element*, bool);
    void activeVideoTextSignal(Element*, const QString&);
    void activeTabTextSignal(void*);
    void activeTabMusicSignal(GlobalMusicAttr*);
    void assignTabValueSignal();
    //void updatedElementTextSignal(const QString&);
    void saveMoiveProgress(qint64 completed, qint64 total);
    void saveMoiveFinish();
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
    void selectedPanZoom(const QString& panzoom);
    void elementAttrChanged(int attrType, bool bPlay);
    void durationChanged(qint64 duration);
    void musicDurationChanged(qint64 duration);
    void positionChanged(qint64 position);
    //void selectedText(void*);
    //void handleContextMenuRequested(const QPoint &pos);
    void updatedText(const QString& qsAss, const QString& qsText);
    void videoStateChanged(QMediaPlayer::State);
    void saveVideo();
    void publishVideo();
    void newProject();
    void openProject();
    void saveProject();
    void cleanProject();
private:
    FlowLayout *m_flowLayout;
    QVBoxLayout *m_firstLayout;
    QLabel* m_firstLabel;

    //进度条竖线
    QFrame *m_vecticalLine; 
    qint64 m_imgWidth;
    qint64 m_signalImgWidth;
    qint64 m_imgHeight;
    qint64 m_lineWidth;
    qint64 m_playPosition;
    //end
    qint64 m_iTotalVideoDuration;
    QSize m_qFinalVideoSize; //最终视频统一尺寸

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
    //uint8_t* m_pTextVideoOutBuffer; // 包含文字的视频
    //size_t m_textVideoOutLen;
    //size_t m_textVideoMaxOutLen;
    uint8_t* m_pOutBufferMusic;
    size_t m_outlenMusic;
    size_t m_outMaxLenMusic;
    QByteArray m_playDataMusic;
    QBuffer m_playBufferMusic;

    GlobalContext* m_globalContext;

    QByteArray m_qbAss;

public:
    //music
    QMediaPlayer m_musicMediaPlayer;
private:

    GlobalMusicAttr* m_globalMusicAttr;
    bool m_isFirstMusic;
    //QString m_qsMusicFilename;
    //int m_iMusicStart;
    //int m_iMusicDuration;
    
    //video


    QString m_qsVideoFileFormat;
    QString m_qsFinalVideoFileFormat;

    QString m_qsMusicFileFormat;

    QString m_qsProjectFile;
    bool m_bCurrentProjectChanged;
    QString m_qsFileType;
};
//! [0]
#include <QProgressDialog>
class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QString& qsFileName, QWidget *parent = Q_NULLPTR);

public slots:
    void saveMoiveProgress(qint64 completed, qint64 total);
};

#endif // ELEMENTSEDIT_H
