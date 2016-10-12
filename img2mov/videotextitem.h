#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include "comm.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    GraphicsTextItem(QGraphicsItem *parent = 0);
    stTextAttr* textAttr()
    {
        return m_stTextAttr;
    }
    void setTextAttr(stTextAttr* stTextAttr){m_stTextAttr=stTextAttr;}
    bool getChanged(){return m_changed;}
signals:
    void lostFocus(GraphicsTextItem *item);
    void selectedChange(QGraphicsItem *item);
public slots:
    void documentChanged();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    //void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
private:
    void hitFrame(QGraphicsSceneHoverEvent *event);
private:
    stTextAttr* m_stTextAttr;
    bool m_changed;
    QFont m_oldFont;
};
//! [0]

#endif // DIAGRAMTEXTITEM_H
