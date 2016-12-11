#include "eresizefocus.h"
#include <QGraphicsItem>

EResizeFocus::EResizeFocus(PosInHost pos,QGraphicsItem *parent)
	: QGraphicsRectItem(0,0,6,6,parent),posInHost(pos),wsize(6)
{
    setAcceptHoverEvents(true);
	setVisible(false);
	locateInHost();
}
EResizeFocus::EResizeFocus(qreal len, PosInHost pos, QGraphicsItem *parent)
	:QGraphicsRectItem(0,0,len,len,parent),posInHost(pos),wsize(len)
{
    setAcceptHoverEvents(true);
	setVisible(false);
	locateInHost();
}
EResizeFocus::~EResizeFocus()
{
}
void EResizeFocus::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	switch(posInHost){
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
void EResizeFocus::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
	QGraphicsRectItem::hoverLeaveEvent(event);
}
void EResizeFocus::locateInHost()
{
	const QRectF parentRect = this->parentItem()->boundingRect();
	qreal x = 0, y = 0;
	switch(posInHost){
		case NORTH_MIDDLE:
			x = parentRect.width() / 2 - wsize / 2;
			y = 0;
			//y = - wsize / 2;
			break;
		case SOUTH_MIDDLE:
			x = parentRect.width() / 2 - wsize / 2;
			y = parentRect.height() - wsize;
			//y = parentRect.height() - wsize/2;
			break;
		case EAST_MIDDLE:
			x = parentRect.width() - wsize;
			//x = parentRect.width() - wsize/2;
			y = parentRect.height() / 2 - wsize / 2;
			break;
		case WEST_MIDDLE:
			x = 0;
			//x = - wsize / 2;
			y = parentRect.height() / 2 - wsize / 2;
			break;
		case NORTH_WEST:
			x = 0;
			//x = - wsize / 2;
			y = 0;
			//y = - wsize / 2;
			break;
		case SOUTH_EAST:
			x = parentRect.width() - wsize;
			//x = parentRect.width() - wsize/2;
			y = parentRect.height() - wsize;
			//y = parentRect.height() - wsize/2;
			break;
		case NORTH_EAST:
			x = parentRect.width() - wsize;
			//x = parentRect.width() - wsize/2;
			y = 0;
			//y = - wsize / 2;
			break;
		case SOUTH_WEST:
			x = 0;
			//x = - wsize / 2;
			y = parentRect.height() - wsize;
			//y = parentRect.height() - wsize/2;
			break;
		default:
			break;
	}
	setPos(x,y);
}
