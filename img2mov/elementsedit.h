#ifndef ELEMENTSEDIT_H
#define ELEMENTSEDIT_H

#include <QWidget>
#include <QScrollArea>
#include "flowlayout.h"
#include "element.h"
#include <QBuffer>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
//! [0]
class ElementsEdit : public QWidget
//class ElementsEdit : public QScrollArea
{
    Q_OBJECT

public:
    ElementsEdit(QWidget *parent = 0);
    ~ElementsEdit();
private:
    int callFfmpeg(const QVector<QString>& vqsArgv);
    void createFinalVideoAndPlay(bool bPlay=false);
    void scaleImage(Element *element);
    void createVideo(Element *element);
    bool createAnimation(Element *firstElement, Element *secondElement
            , const QString& animationName);
    void initialProgress();
    void assignProgress();
signals:
    void readyVideo(const QString& fileName, const QByteArray& buffer, int position);
    void changePlayPosition(int);
    void playVideo();
    void createTextSignal(void*);
    void displayTextSignal(void*, bool);
    void activeVideoTextSignal(void*, const QString&);
    void activeTabTextSignal(void*);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
public slots:
    void load();
    void selectedImage();
    void selectedText(const QString&);
    void selectedTransition(const QString& animation);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    //void selectedText(void*);
    //void handleContextMenuRequested(const QPoint &pos);
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
};
//! [0]

#endif // ELEMENTSEDIT_H
