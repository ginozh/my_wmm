#include "videotextitem.h"
#include "videoscene.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QGraphicsSceneHoverEvent>
#include <QTextDocument>

//! [0]
GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);

    m_stTextAttr = 0;

    m_changed = false;


    connect((const QObject *)document(), SIGNAL( contentsChanged()),
            this, SLOT(documentChanged()));
#if 0
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignHCenter);
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    setTextCursor(cursor);
#endif
    QTextDocument *tdocument =  document();
    tdocument->setModified(true);
    QTextOption option = tdocument->defaultTextOption();
    option.setAlignment(Qt::AlignHCenter);
    tdocument->setDefaultTextOption(option);
    //setTextWidth(tdocument->idealWidth());
    setTextWidth(400);
}
//! [0]
//! [1]
QVariant GraphicsTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::itemChange")));
        //emit selectedChange(this);
    }
    if(change == QGraphicsItem::ItemPositionHasChanged ||
            change == QGraphicsItem::ItemTransformHasChanged
            )
    {
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::itemChange")));
        m_changed = true;
    }
    return value;
}
#if 1
void GraphicsTextItem::focusInEvent(QFocusEvent *event)
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::focusInEvent")));
    //setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::focusInEvent(event);
    m_changed = false;
    m_oldFont = font();
}
#endif
//! [2]
void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::focusOutEvent scene_w: %1 scene_h: %2 pos_x: %3 pos_y: %4 text_w: %5")).arg(scene()->width()).arg(scene()->height()).arg(x()).arg(y()).arg(textWidth()));
    //setTextInteractionFlags(Qt::NoTextInteraction);
    if(m_oldFont != font())
    {
        m_changed = true;
    }
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
#if 0
//! [2]
void GraphicsTextItem::keyPressEvent(QKeyEvent *keyEvent) 
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::eventFilter key: %1 ")).arg(keyEvent->text()));
    //qDebug( "GraphicsTextItem::keyPressEvent key press %d", keyEvent->key() );
    QGraphicsTextItem::keyPressEvent(keyEvent);
    //tmptextItem->setFocus();
}
#endif
#if 0
void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::paint")));
    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    //o->state |= QStyle::State_HasFocus;
    //o->state &= QStyle::State_UpArrow;
    QGraphicsTextItem::paint(painter, o, widget);
}
#endif
void GraphicsTextItem::hitFrame(QGraphicsSceneHoverEvent *event)
{
    if(hasFocus())
    {
        //setCursor(Qt::SizeAllCursor);
        //QMessageBox::information(NULL, "info", QString(tr("hoverMoveEvent")));
        QPointF curPoint(event->scenePos());
        qreal x=curPoint.x(),y=curPoint.y();

        const QRectF parentRect = boundingRect();
        QPointF topLeft = parentRect.topLeft();
        qreal xr = pos().x();//topLeft.x();
        qreal yr = pos().y();//topLeft.y();
        qreal wr = parentRect.width();
        qreal hr = parentRect.height();

        //QMessageBox::information(NULL, "info", QString(tr("hoverEnterEvent. x: %1 y: %2 xr: %3 yr: %4 wr: %5 hr: %6")).arg(x).arg(y).arg(xr).arg(yr).arg(wr).arg(hr));

        qreal tmpMargin=3;
        if( ( ( (x>=xr && x<=(xr+tmpMargin)) || (x>=(xr+wr-tmpMargin) && x<=(xr+wr)) ) && (y>=yr && y<=(yr+hr) ))
         || ( ( (y>=yr && y<=(yr+tmpMargin)) || (y>=(yr+hr-tmpMargin) && y<=(yr+hr)) ) && (x>=xr && x<=(xr+wr) )) )
        {
            setCursor(Qt::SizeAllCursor);
            //QMessageBox::information(NULL, "info", QString(tr("hoverEnterEvent. x: %1 y: %2 xr: %3 yr: %4 wr: %5 hr: %6")).arg(x).arg(y).arg(xr).arg(yr).arg(wr).arg(hr));
            //QMessageBox::information(NULL, "info", QString(tr("hoverEnterEvent")));
        }
        else
        {
            setCursor(Qt::IBeamCursor);
        }
    }
}
void GraphicsTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::hoverEnterEvent")));
    hitFrame(event);
    QGraphicsTextItem::hoverEnterEvent(event);

}
void GraphicsTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    hitFrame(event);
    QGraphicsTextItem::hoverMoveEvent(event);
}
void GraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hitFrame(event);
    QGraphicsTextItem::hoverLeaveEvent(event);
}
void GraphicsTextItem::documentChanged()
{
    m_changed = true;
}
