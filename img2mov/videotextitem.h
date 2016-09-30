#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QPen>
#include "comm.h"

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    DiagramTextItem(QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE { return Type; }

    stTextAttr* textAttr()
    {
        //if(m_stTextAttr->m_qsText.compare(toPlainText()))
        {
            //m_stTextAttr->m_qsText = toPlainText() ;
        }
        return m_stTextAttr;
    }
    void setTextAttr(stTextAttr* stTextAttr){m_stTextAttr=stTextAttr;}

signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    stTextAttr* m_stTextAttr;
};
//! [0]

#endif // DIAGRAMTEXTITEM_H
