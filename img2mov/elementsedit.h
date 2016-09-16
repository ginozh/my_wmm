#ifndef ELEMENTSEDIT_H
#define ELEMENTSEDIT_H

#include <QWidget>
#include <QScrollArea>
#include "flowlayout.h"
#include "element.h"
#include <QBuffer>

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
    void createFinalVideo();
    void scaleImage(Element *element);
    void createVideo(Element *element);
signals:
    void playVideo(const QByteArray&);

public slots:
    void load();
    void selectedImage();
    void selectedTransition();
    //void handleContextMenuRequested(const QPoint &pos);
private:
    FlowLayout *m_flowLayout;
    QFrame *m_vecticalLine;
    QString m_tmpdir;
    QWidget *m_lastSelectedImage;
    int m_idxCurrentImage;
    QByteArray m_playData;
    QBuffer m_playBuffer;
};
//! [0]

#endif // ELEMENTSEDIT_H
