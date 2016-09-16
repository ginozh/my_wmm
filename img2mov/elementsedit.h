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
    void createFinalVideoAndPlay();
    void scaleImage(Element *element);
    void createVideo(Element *element);
    bool createAnimation(Element *firstElement, Element *secondElement
            , const QString& animationName);
    void initialProgress();
signals:
    void playVideo(const QByteArray&);
    void verticalLineMoved(int);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public slots:
    void load();
    void selectedImage();
    void selectedTransition();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    //void handleContextMenuRequested(const QPoint &pos);
private:
    FlowLayout *m_flowLayout;
    QVBoxLayout *m_firstLayout;
    QLabel* m_firstLabel;

    QFrame *m_vecticalLine; //进度条竖线
    qint64 m_duration;
    qint64 m_imgWidth;

    QString m_tmpdir;
    QWidget *m_lastSelectedImage;
    int m_idxCurrentImage;
    QByteArray m_playData;
    QBuffer m_playBuffer;

    bool m_isFirstClick;
};
//! [0]

#endif // ELEMENTSEDIT_H
