#include "videorectitem.h"
#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QBrush>
#include "comm.h"

GraphicsRectItem::GraphicsRectItem(PosInHost pos,QGraphicsItem *parent)
	: m_wsize(6*GlobalContext::instance()->m_dFactorX), QGraphicsRectItem(0,0,m_wsize,m_wsize,parent),m_posInHost(pos)
{
    setAcceptHoverEvents(true);
	setVisible(false);
	locateInHost();
    setBrush(QBrush(Qt::white));
}
GraphicsRectItem::GraphicsRectItem(qreal len, PosInHost pos, QGraphicsItem *parent)
	:QGraphicsRectItem(0,0,len,len,parent),m_posInHost(pos),m_wsize(len)
{
    setAcceptHoverEvents(true);
	setVisible(false);
	locateInHost();
    setBrush(QBrush(Qt::white));
}
GraphicsRectItem::~GraphicsRectItem()
{
}
void GraphicsRectItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	switch(m_posInHost){
		case NORTH_MIDDLE:
			setCursor(Qt::SizeVerCursor);
			break;
		case SOUTH_MIDDLE:
			setCursor(Qt::SizeVerCursor);
			break;
		case EAST_MIDDLE:
			setCursor(Qt::SizeHorCursor);
			break;
		case WEST_MIDDLE:
			setCursor(Qt::SizeHorCursor);
			break;
		case NORTH_WEST:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case SOUTH_EAST:
			setCursor(Qt::SizeFDiagCursor);
			break;
		case NORTH_EAST:
			setCursor(Qt::SizeBDiagCursor);
			break;
		case SOUTH_WEST:
			setCursor(Qt::SizeBDiagCursor);
			break;
		default:
			break;
	}
	QGraphicsRectItem::hoverEnterEvent(event);
}
void GraphicsRectItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
	QGraphicsRectItem::hoverLeaveEvent(event);
}
void GraphicsRectItem::locateInHost()
{
	const QRectF parentRect = this->parentItem()->boundingRect();
	qreal x = 0, y = 0;
	switch(m_posInHost){
		case NORTH_MIDDLE:
			x = parentRect.width() / 2 - m_wsize / 2;
			y = 0;
			//y = - m_wsize / 2;
			break;
		case SOUTH_MIDDLE:
			x = parentRect.width() / 2 - m_wsize / 2;
			y = parentRect.height() - m_wsize;
			//y = parentRect.height() - m_wsize/2;
			break;
		case EAST_MIDDLE:
			x = parentRect.width() - m_wsize;
			//x = parentRect.width() - m_wsize/2;
			y = parentRect.height() / 2 - m_wsize / 2;
			break;
		case WEST_MIDDLE:
			x = 0;
			//x = - m_wsize / 2;
			y = parentRect.height() / 2 - m_wsize / 2;
			break;
		case NORTH_WEST:
			x = 0;
			//x = - m_wsize / 2;
			y = 0;
			//y = - m_wsize / 2;
			break;
		case SOUTH_EAST:
			x = parentRect.width() - m_wsize;
			//x = parentRect.width() - m_wsize/2;
			y = parentRect.height() - m_wsize;
			//y = parentRect.height() - m_wsize/2;
			break;
		case NORTH_EAST:
			x = parentRect.width() - m_wsize;
			//x = parentRect.width() - m_wsize/2;
			y = 0;
			//y = - m_wsize / 2;
			break;
		case SOUTH_WEST:
			x = 0;
			//x = - m_wsize / 2;
			y = parentRect.height() - m_wsize;
			//y = parentRect.height() - m_wsize/2;
			break;
		default:
			break;
	}
	setPos(x,y);
}
void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
#if 0
    QPen pen(Qt::white);
    pen.setWidth(1);
    //pen.setStyle(Qt::DashLine);
    painter->setPen( pen );
    painter->drawRect( option->rect );
#endif
    //QGraphicsRectItem::paint(painter, option, widget);
    painter->fillRect(option->rect ,Qt::white);
}
