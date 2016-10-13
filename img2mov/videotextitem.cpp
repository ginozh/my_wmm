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
    ,width(100),height(100),margin(8)
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


	createGraphicsRectItem();
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
void GraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	curResizeFocus = qgraphicsitem_cast<GraphicsRectItem*>(scene()->itemAt(event->scenePos(),sceneTransform() ));
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
void GraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
		GraphicsRectItem::PosInHost pos = curResizeFocus->getInHost();
		switch(pos){
			case GraphicsRectItem::NORTH_MIDDLE:
				curY += hChanging; curHeight-=hChanging;
				break;
			case GraphicsRectItem::SOUTH_MIDDLE:
				curHeight+=hChanging;
				break;
			case GraphicsRectItem::EAST_MIDDLE:
				curWidth+=wChanging;
				break;
			case GraphicsRectItem::WEST_MIDDLE:
				curX+=wChanging; curWidth-=wChanging;
				break;
			case GraphicsRectItem::NORTH_WEST:
				curX+=wChanging; curY+=hChanging; curWidth-=wChanging; curHeight-=hChanging;
				break;
			case GraphicsRectItem::SOUTH_EAST:
				curWidth+=wChanging; curHeight+=hChanging;
				break;
			case GraphicsRectItem::NORTH_EAST:
				curY+=hChanging; curWidth+=wChanging; curHeight-=hChanging;
				break;
			case GraphicsRectItem::SOUTH_WEST:
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
void GraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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
#if 0
        QTextDocument *tdocument =  document();
        tdocument->setModified(true);
        QTextOption option = tdocument->defaultTextOption();
        option.setAlignment(Qt::AlignHCenter);
        tdocument->setDefaultTextOption(option);
#endif
        setTextWidth(width);

		//scene()->removeItem(dashRect);
		delete dashRect;
        //scene()->update(scene()->sceneRect()); // 需要增加，否则原来的边框不会消失
	}
	else
		QGraphicsItem::mouseReleaseEvent(event);
}
void GraphicsTextItem::showResizeFocus(bool visible)
{
	for(int i = 0; i < resizeFocus.count(); i++)
	{
		resizeFocus.at(i)->locateInHost();
		resizeFocus.at(i)->setVisible(visible);
	}
}
QRectF GraphicsTextItem::boundingRect() const
{
    //QMessageBox::information(NULL, "info", QString(tr("boundingRect w: %1 h: %2")).arg(width).arg(height));
    //qDebug("boundingRect\n");
	return QRectF(0, 0, width, height);
}
void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	Q_UNUSED(widget);
	QColor fillColor = QColor(100,100,255);
	if (option->state & QStyle::State_Selected) fillColor = fillColor.darker(150);
	if (option->state & QStyle::State_MouseOver) fillColor = fillColor.lighter(125);
	if(option->state & QStyle::State_Selected ||
            option->state & QStyle::State_HasFocus)
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
QPainterPath GraphicsTextItem::shape() const
{
	QPainterPath path;
	path.addRect(0,0,width,height);
	return path;
}
void GraphicsTextItem::createGraphicsRectItem()
{
	GraphicsRectItem *north_middle = new GraphicsRectItem(margin,GraphicsRectItem::NORTH_MIDDLE,this);
	resizeFocus.append(north_middle);
	GraphicsRectItem *north_east = new GraphicsRectItem(margin,GraphicsRectItem::NORTH_EAST,this);
	resizeFocus.append(north_east);
	GraphicsRectItem *north_west = new GraphicsRectItem(margin,GraphicsRectItem::NORTH_WEST,this);
	resizeFocus.append(north_west);
	GraphicsRectItem *south_middle = new GraphicsRectItem(margin,GraphicsRectItem::SOUTH_MIDDLE,this);
	resizeFocus.append(south_middle);
	GraphicsRectItem *south_east = new GraphicsRectItem(margin,GraphicsRectItem::SOUTH_EAST,this);
	resizeFocus.append(south_east);
	GraphicsRectItem *south_west = new GraphicsRectItem(margin,GraphicsRectItem::SOUTH_WEST,this);
	resizeFocus.append(south_west);
	GraphicsRectItem *east_middle = new GraphicsRectItem(margin,GraphicsRectItem::EAST_MIDDLE,this);
	resizeFocus.append(east_middle);
	GraphicsRectItem *west_middle = new GraphicsRectItem(margin,GraphicsRectItem::WEST_MIDDLE,this);
	resizeFocus.append(west_middle);
}
