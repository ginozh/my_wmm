#ifndef ELEMENTSEDIT_H
#define ELEMENTSEDIT_H

#include <QWidget>
#include <QScrollArea>
#include "flowlayout.h"

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
signals:
    void playVideo(const QString&);

public slots:
    void load();
    void selectedImage();
    void selectedTransition();
    //void handleContextMenuRequested(const QPoint &pos);
private:
    FlowLayout *m_flowLayout;
    QString m_tmpdir;
    QWidget *m_lastSelectedImage;
    int m_idxCurrentImage;
};
//! [0]

#endif // ELEMENTSEDIT_H
