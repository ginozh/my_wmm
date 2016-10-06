#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "videotextitem.h"

#include <QGraphicsScene>
#include <QMap>
#include "comm.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit DiagramScene(QObject *parent = 0);
    void setFont(void* element, const QFont &font);
    void setTextAttr(void* element, stTextAttr *textAttr);

public slots:
    //void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(DiagramTextItem *item);
    void createText(void*);
    void displayVideoText(void*, bool);
    void activeVideoText(void* element, const QString&);

signals:
    void updatedTextSignal(stTextAttr*, const QString&);

protected:
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;

private:
    
    QMap<void*, DiagramTextItem*> m_mapText;
    DiagramTextItem *curtextItem;

    QFont myFont;
    QColor myTextColor;
};
//! [0]

#endif // DIAGRAMSCENE_H
