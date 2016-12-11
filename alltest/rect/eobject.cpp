#include "eobject.h"
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDebug>

EObject::EObject(QGraphicsItem *parent)
	:QGraphicsTextItem(parent)
    ,width(100),height(100),margin(8)
{
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setPlainText("Qt Everywhere!");

    QTextDocument *tdocument =  document();
    tdocument->setModified(true);
    QTextOption option = tdocument->defaultTextOption();
    option.setAlignment(Qt::AlignHCenter);
    tdocument->setDefaultTextOption(option);

    setTextInteractionFlags(Qt::TextEditorInteraction);
	createResizeFocus();
}
EObject::EObject(qreal w,qreal h,qreal m, QGraphicsItem *parent)
	:QGraphicsTextItem(parent)
	,width(w),height(h),margin(m)
{
	setAcceptDrops(true);
	setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setPlainText("Qt Everywhere!");


    QTextDocument *tdocument =  document();
    tdocument->setModified(true);
    QTextOption option = tdocument->defaultTextOption();
    option.setAlignment(Qt::AlignHCenter);
    tdocument->setDefaultTextOption(option);


	createResizeFocus();
}
#if 0
EObject::~EObject()
{
}
#endif
QRectF EObject::boundingRect() const
{
    //QMessageBox::information(NULL, "info", QString(tr("boundingRect w: %1 h: %2")).arg(width).arg(height));
    qDebug("boundingRect\n");
	return QRectF(0, 0, width, height);
}
void EObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	Q_UNUSED(widget);
	QColor fillColor = QColor(100,100,255);
	if (option->state & QStyle::State_Selected) fillColor = fillColor.darker(150);
	if (option->state & QStyle::State_MouseOver) fillColor = fillColor.lighter(125);
	if(option->state & QStyle::State_Selected)
	{
		/*painter->fillRect(margin, margin, width-margin*2, height-margin*2 ,fillColor);
		painter->setPen(Qt::DotLine);
		painter->drawRect(margin, margin, width-margin*2, height-margin*2);*/
		showResizeFocus(true);
	}
	else
	{
		/*painter->fillRect(margin, margin, width-margin*2, height-margin*2 ,fillColor);
		painter->drawRect(margin, margin, width-margin*2, height-margin*2);*/
		showResizeFocus(false);
	}
#if 0
    //如果大小超过字的大小，则缩放height
    QFont font;
    QFontMetrics fontMetrics(font);
    int fh = fontMetrics.height();
    QString sceneText = toPlainText();
    int sceneTextWidth = fontMetrics.width(sceneText);
    if(width>=sceneTextWidth)
    {
        height=(sceneTextWidth/width+1)*fh;
    }
    else
    {
        //把当前行的最后一个字放在下一行中去
    }
#endif
    scene()->update(scene()->sceneRect()); // 需要增加，否则原来的边框不会消失

    QGraphicsTextItem::paint(painter, option, widget);
}
QPainterPath EObject::shape() const
{
	QPainterPath path;
	path.addRect(0,0,width,height);
	return path;
}
void EObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	curResizeFocus = qgraphicsitem_cast<EResizeFocus*>(scene()->itemAt(event->scenePos(),sceneTransform() ));
	if(curResizeFocus)
	{
		myMode = RESIZE;
		lastPoint.setX(event->scenePos().x());
		lastPoint.setY(event->scenePos().y());
		dashRect = new QGraphicsRectItem();
		dashRect->setPen(QPen(Qt::DashLine));
		//dashRect->setPen(QPen(Qt::SolidLine));
		dashRect->setParentItem(this);
		//dashRect->setRect(margin, margin, width-margin*2, height-margin*2);
		dashRect->setRect(0, 0, width, height);
        //scene()->addItem(dashRect);


        //clearFocus();
	}
	else
	{
		myMode = MOVE;
		QGraphicsItem::mousePressEvent(event);
	}
}
void EObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(myMode == RESIZE)
	{
		QPointF curPoint(event->scenePos());
        QRectF sceneRect = scene()->sceneRect();
        qreal x=curPoint.x(),y=curPoint.y();
#if 0
        if(sceneRect.width()<x ||sceneRect.height()<y)
        {
            QMessageBox::information(NULL, "info", QString(tr("mouseMoveEvent w: %1 h: %2 pos_x: %3 pos_y: %4")).arg(sceneRect.width()).arg(sceneRect.height()).arg(x).arg(y));
            if(sceneRect.width()<x)
                x = sceneRect.width();
            if(sceneRect.height()<y)
                y = sceneRect.height();
        }
#endif
		//qreal curX = margin, curY = margin, curWidth = width-margin*2, curHeight = height-margin*2;
		qreal curX = 0, curY = 0, curWidth = width, curHeight = height;
		qreal wChanging = x-lastPoint.x(), hChanging = y-lastPoint.y();
		EResizeFocus::PosInHost pos = curResizeFocus->getInHost();
		switch(pos){
			case EResizeFocus::NORTH_MIDDLE:
				curY += hChanging; curHeight-=hChanging;
				break;
			case EResizeFocus::SOUTH_MIDDLE:
				curHeight+=hChanging;
				break;
			case EResizeFocus::EAST_MIDDLE:
				curWidth+=wChanging;
				break;
			case EResizeFocus::WEST_MIDDLE:
				curX+=wChanging; curWidth-=wChanging;
				break;
			case EResizeFocus::NORTH_WEST:
				curX+=wChanging; curY+=hChanging; curWidth-=wChanging; curHeight-=hChanging;
				break;
			case EResizeFocus::SOUTH_EAST:
				curWidth+=wChanging; curHeight+=hChanging;
				break;
			case EResizeFocus::NORTH_EAST:
				curY+=hChanging; curWidth+=wChanging; curHeight-=hChanging;
				break;
			case EResizeFocus::SOUTH_WEST:
				curX+=wChanging; curWidth-=wChanging; curHeight+=hChanging;
				break;
			default:
				break;
		}
		if(curWidth < 20 ||curHeight <20) return; //!minimal size
		dashRect->setRect(curX,curY,curWidth,curHeight);

        //scene()->update(scene()->sceneRect());
        //prepareGeometryChange();
	}
	else
    {
        //QMessageBox::information(NULL, "info", QString(tr("mouseMoveEvent")));
		QGraphicsItem::mouseMoveEvent(event);
    }
}
void EObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(myMode == RESIZE)
	{
		//width = dashRect->rect().width() +margin*2;
		width = dashRect->rect().width() ;
		//height = dashRect->rect().height() +margin*2;
		height = dashRect->rect().height() ;
		//qreal curX = dashRect->rect().left() - margin;
        //

		qreal curX = dashRect->rect().left();
		//qreal curY = dashRect->rect().top() - margin;
		qreal curY = dashRect->rect().top();
		QPointF curPos(curX,curY);
		setPos(mapToScene(curPos));
		//scene()->removeItem(dashRect);
		delete dashRect;
        //scene()->update(scene()->sceneRect()); // 需要增加，否则原来的边框不会消失
	}
	else
		QGraphicsItem::mouseReleaseEvent(event);
}
void EObject::showResizeFocus(bool visible)
{
	for(int i = 0; i < resizeFocus.count(); i++)
	{
		resizeFocus.at(i)->locateInHost();
		resizeFocus.at(i)->setVisible(visible);
	}
}
void EObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hitFrame(event);
    QGraphicsTextItem::hoverEnterEvent(event);
}
void EObject::hitFrame(QGraphicsSceneHoverEvent *event)
{
    if(hasFocus())
    {
        //setCursor(Qt::SizeAllCursor);
        //QMessageBox::information(NULL, "info", QString(tr("hoverMoveEvent")));
        QPointF curPoint(event->scenePos());
        qreal x=curPoint.x(),y=curPoint.y();

        const QRectF parentRect = boundingRect();
        QPointF topLeft = parentRect.topLeft();
        qreal xr = topLeft.x();
        qreal yr = topLeft.y();
        qreal wr = parentRect.width();
        qreal hr = parentRect.height();

        //QMessageBox::information(NULL, "info", QString(tr("hoverEnterEvent. x: %1 y: %2 xr: %3 yr: %4 wr: %5 hr: %6")).arg(x).arg(y).arg(xr).arg(yr).arg(wr).arg(hr));

        if((( (x>=xr&&x<=(xr+2)) || x==(xr+wr)) && (y>=yr && y<=(yr+hr) ))
         || ( (y==yr || y==(yr+hr) ) && (x>=xr && x<=(xr+wr) ) ) )
        {
            setCursor(Qt::SizeAllCursor);
            //QMessageBox::information(NULL, "info", QString(tr("hoverEnterEvent")));
        }
        else
        {
            setCursor(Qt::IBeamCursor);
        }
    }
}
void EObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    hitFrame(event);
    QGraphicsTextItem::hoverMoveEvent(event);
}
void EObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hitFrame(event);
    QGraphicsTextItem::hoverLeaveEvent(event);
}
void EObject::createResizeFocus()
{
	EResizeFocus *north_middle = new EResizeFocus(margin,EResizeFocus::NORTH_MIDDLE,this);
	resizeFocus.append(north_middle);
	EResizeFocus *north_east = new EResizeFocus(margin,EResizeFocus::NORTH_EAST,this);
	resizeFocus.append(north_east);
	EResizeFocus *north_west = new EResizeFocus(margin,EResizeFocus::NORTH_WEST,this);
	resizeFocus.append(north_west);
	EResizeFocus *south_middle = new EResizeFocus(margin,EResizeFocus::SOUTH_MIDDLE,this);
	resizeFocus.append(south_middle);
	EResizeFocus *south_east = new EResizeFocus(margin,EResizeFocus::SOUTH_EAST,this);
	resizeFocus.append(south_east);
	EResizeFocus *south_west = new EResizeFocus(margin,EResizeFocus::SOUTH_WEST,this);
	resizeFocus.append(south_west);
	EResizeFocus *east_middle = new EResizeFocus(margin,EResizeFocus::EAST_MIDDLE,this);
	resizeFocus.append(east_middle);
	EResizeFocus *west_middle = new EResizeFocus(margin,EResizeFocus::WEST_MIDDLE,this);
	resizeFocus.append(west_middle);
}
