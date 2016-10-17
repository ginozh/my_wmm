#include "videotextitem.h"
#include "videoscene.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QGraphicsSceneHoverEvent>
#include <QTextDocument>
#include <QDebug>

//! [0]
GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
    ,m_width(100),m_height(100),m_margin(8)
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
    option.setWrapMode(QTextOption::NoWrap);
    tdocument->setDefaultTextOption(option);
    //setTextWidth(tdocument->idealWidth());
    ////setTextWidth(400);


	createGraphicsRectItem();
}
void GraphicsTextItem::setTextAttr(stTextAttr* stTextAttr)
{
    m_stTextAttr=stTextAttr;

    if(m_stTextAttr)
    {
        setFont(m_stTextAttr->m_qfont);
        setDefaultTextColor(m_stTextAttr->m_fontColor);

        QTextDocument *tdocument =  document();
        QTextOption option = tdocument->defaultTextOption();
        if(m_stTextAttr->m_textAlign>=0)
        {
            option.setAlignment((Qt::AlignmentFlag)m_stTextAttr->m_textAlign);
        }
        tdocument->setDefaultTextOption(option);
    }
}
//! [0]
void GraphicsTextItem::setFirstTextPosWH(const QString& oritxt)
{
    if(toPlainText().isEmpty())
    {
        QFont font;
        QFontMetrics fontMetrics(font);
        int fw = fontMetrics.width(oritxt);
        int fh = fontMetrics.height();
        m_width=fw+20;
        m_height=fh+10;

        setTextWidth(m_width);
        int x=scene()->width()/2-textWidth()/2;
        int y=scene()->height()*3/5;
        setPos(x, y);
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::setFirstTextPosWH x: %1 y: %2")).arg(x).arg(y));

        setPlainText(oritxt);
    }
}
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
    if(m_changed)
    {
        createAssInfo();
        emit lostFocus(this);
        m_changed = false;
    }
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
#if 1
void GraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_curGraphicsRectItem = qgraphicsitem_cast<GraphicsRectItem*>(scene()->itemAt(event->scenePos(),sceneTransform() ));
	if(m_curGraphicsRectItem)
	{
        qDebug()<< "mousePressEvent: RESIZE";
        //QMessageBox::information(NULL, "info", QString(tr("mousePressEvent resize")));
		m_mode = RESIZE;
		m_lastPoint.setX(event->scenePos().x());
		m_lastPoint.setY(event->scenePos().y());
		m_dashRect = new QGraphicsRectItem();
		m_dashRect->setPen(QPen(Qt::DashLine));
		//m_dashRect->setPen(QPen(Qt::SolidLine));
		m_dashRect->setParentItem(this);
		//m_dashRect->setRect(m_margin, m_margin, width-m_margin*2, m_height-m_margin*2);
		m_dashRect->setRect(0, 0, m_width, m_height);
        //scene()->addItem(m_dashRect);


        //clearFocus();
	}
	else
	{
        qDebug()<< "mousePressEvent: MOVE";
		m_mode = MOVE;
        //QMessageBox::information(NULL, "info", QString(tr("mousePressEvent move")));
		QGraphicsTextItem::mousePressEvent(event);
	}
}
void GraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(m_mode == RESIZE)
	{
        //qDebug()<< "mouseMoveEvent: RESIZE";
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
		//qreal curX = m_margin, curY = m_margin, curWidth = m_width-m_margin*2, curHeight = m_height-m_margin*2;
		qreal curX = 0, curY = 0, curWidth = m_width, curHeight = m_height;
		qreal wChanging = x-m_lastPoint.x(), hChanging = y-m_lastPoint.y();
		GraphicsRectItem::PosInHost pos = m_curGraphicsRectItem->getInHost();
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
		m_dashRect->setRect(curX,curY,curWidth,curHeight);

        //scene()->update(scene()->sceneRect());
        //prepareGeometryChange();
	}
	else
    {
        //qDebug()<< "mouseMoveEvent: MOVE";
        //QMessageBox::information(NULL, "info", QString(tr("mouseMoveEvent")));
		QGraphicsTextItem::mouseMoveEvent(event);
    }
}
void GraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(m_mode == RESIZE)
	{
        //qDebug(QString(tr("GraphicsTextItem::mouseReleaseEvent: RESIZE text: %1")).arg(toPlainText()).toStdString().c_str());
		//width = m_dashRect->rect().width() +m_margin*2;
		m_width = m_dashRect->rect().width() ;
		//m_height = m_dashRect->rect().height() +m_margin*2;
		m_height = m_dashRect->rect().height() ;
		//qreal curX = m_dashRect->rect().left() - m_margin;
        //

		qreal curX = m_dashRect->rect().left();
		//qreal curY = m_dashRect->rect().top() - m_margin;
		qreal curY = m_dashRect->rect().top();
		QPointF curPos(curX,curY);
		setPos(mapToScene(curPos));
#if 0
        QTextDocument *tdocument =  document();
        tdocument->setModified(true);
        QTextOption option = tdocument->defaultTextOption();
        option.setAlignment(Qt::AlignHCenter);
        tdocument->setDefaultTextOption(option);
#endif
        setTextWidth(m_width);

		//scene()->removeItem(m_dashRect);
		delete m_dashRect;
        //scene()->update(scene()->sceneRect()); // 需要增加，否则原来的边框不会消失
	}
	else
    {
        //qDebug(QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE text: %1")).arg(toPlainText()).toStdString().c_str());
        //QMessageBox::information(NULL, "info",QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE linecount: %1 blockCount: %2 text: %3")).arg(document()->lineCount()).arg(document()->blockCount()).arg(toPlainText()));//会有换行
#if 0
        const QString& str = toPlainText();

        QStringList list = str.split('\n');
        foreach (const QString &strline, list) {
            //QMessageBox::information(NULL, "info",QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE text: %1")).arg(strline));
            //QFont font;
            QFontMetrics fontMetrics(m_stTextAttr->m_qfont);
            int fw = fontMetrics.width(strline);
            if(fw>m_width)
            {
                QString strRealLine;
                for (int i = 0; i < strline.size(); ++i) {
                    strRealLine.append(strline.at(i));
                    fw = fontMetrics.width(strRealLine);
                    if(fw>=m_width)
                    {
                        QMessageBox::information(NULL, "info",QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE text: %1")).arg(strRealLine));
                        strRealLine.clear();
                    }
                }
                if(!strRealLine.isEmpty())
                {
                    QMessageBox::information(NULL, "info",QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE text: %1")).arg(strRealLine));
                }
            }
            else
            {
                QMessageBox::information(NULL, "info",QString(tr("GraphicsTextItem::mouseReleaseEvent: MOVE text: %1")).arg(strline));
            }
        }
#endif
		QGraphicsTextItem::mouseReleaseEvent(event);
    }
}
#endif
void GraphicsTextItem::showRectItemFocus(bool visible)
{
	for(int i = 0; i < m_listGraphicsRectItem.count(); i++)
	{
		m_listGraphicsRectItem.at(i)->locateInHost();
		m_listGraphicsRectItem.at(i)->setVisible(visible);
	}
}
QRectF GraphicsTextItem::boundingRect() const
{
    //QMessageBox::information(NULL, "info", QString(tr("boundingRect w: %1 h: %2")).arg(m_width).arg(m_height));
    //qDebug("boundingRect\n");
	return QRectF(0, 0, m_width, m_height);
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
		/*painter->fillRect(m_margin, m_margin, m_width-m_margin*2, m_height-m_margin*2 ,fillColor);
		painter->setPen(Qt::DotLine);
		painter->drawRect(m_margin, m_margin, m_width-m_margin*2, m_height-m_margin*2);*/
		showRectItemFocus(true);
	}
	else
	{
		/*painter->fillRect(m_margin, m_margin, m_width-m_margin*2, m_height-m_margin*2 ,fillColor);
		painter->drawRect(m_margin, m_margin, m_width-m_margin*2, m_height-m_margin*2);*/
		showRectItemFocus(false);
	}
#if 0
    //如果大小超过字的大小，则缩放height
    QFont font;
    QFontMetrics fontMetrics(font);
    int fh = fontMetrics.height();
    QString sceneText = toPlainText();
    int sceneTextWidth = fontMetrics.width(sceneText);
    if(m_width>=sceneTextWidth)
    {
        m_height=(sceneTextWidth/m_width+1)*fh;
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
	path.addRect(0,0,m_width,m_height);
	return path;
}
void GraphicsTextItem::createGraphicsRectItem()
{
	GraphicsRectItem *north_middle = new GraphicsRectItem(m_margin,GraphicsRectItem::NORTH_MIDDLE,this);
	m_listGraphicsRectItem.append(north_middle);
	GraphicsRectItem *north_east = new GraphicsRectItem(m_margin,GraphicsRectItem::NORTH_EAST,this);
	m_listGraphicsRectItem.append(north_east);
	GraphicsRectItem *north_west = new GraphicsRectItem(m_margin,GraphicsRectItem::NORTH_WEST,this);
	m_listGraphicsRectItem.append(north_west);
	GraphicsRectItem *south_middle = new GraphicsRectItem(m_margin,GraphicsRectItem::SOUTH_MIDDLE,this);
	m_listGraphicsRectItem.append(south_middle);
	GraphicsRectItem *south_east = new GraphicsRectItem(m_margin,GraphicsRectItem::SOUTH_EAST,this);
	m_listGraphicsRectItem.append(south_east);
	GraphicsRectItem *south_west = new GraphicsRectItem(m_margin,GraphicsRectItem::SOUTH_WEST,this);
	m_listGraphicsRectItem.append(south_west);
	GraphicsRectItem *east_middle = new GraphicsRectItem(m_margin,GraphicsRectItem::EAST_MIDDLE,this);
	m_listGraphicsRectItem.append(east_middle);
	GraphicsRectItem *west_middle = new GraphicsRectItem(m_margin,GraphicsRectItem::WEST_MIDDLE,this);
	m_listGraphicsRectItem.append(west_middle);
}
#if 1
void GraphicsTextItem::createAssInfo()
{
    if(m_stTextAttr)
    {
        QMessageBox::information(NULL, "info", QString(tr("GraphicsTextItem::createAssInfo. rgb: %1")).arg(m_stTextAttr->m_fontColor.name()));
        int iAlignment=8;
        int iMarginL=10, iMarginR=10, iMarginV=10;
        switch((Qt::AlignmentFlag)m_stTextAttr->m_textAlign)
        {
            case Qt::AlignLeft:
                iAlignment=7;
                iMarginL=pos().x();
                iMarginV=pos().y();
                break;
            case Qt::AlignHCenter:
                iAlignment=7;
                iMarginL=pos().x();
                iMarginV=pos().y();
                break;
            case Qt::AlignRight:
                iAlignment=7;
                iMarginL=pos().x();
                iMarginV=pos().y();
                break;
            default:
                break;
        }
        //"Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour,
        //Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline,
        //Shadow, Alignment, MarginL, MarginR, MarginV, AlphaLevel, Encoding\n"
        m_stTextAttr->m_qsStyle = QString(tr(
        "Style: %1,      %2,       %3,       &Hffffff,      &Hffffff,        &H0,           &H0,       "
        "%4,    %5,      %6,         0,         100,    100,    0,       0,     1,           1,      "
        " 0,      %7,          %8,     %9,     %10,      0,          0\n")).
            //arg((int)this).arg(m_stTextAttr->m_qfont.family()).arg(m_stTextAttr->m_qfont.pointSize()).
            arg((int)this).arg(m_stTextAttr->m_qfont.family()).arg(m_stTextAttr->m_qfont.pixelSize()).
            arg(m_stTextAttr->m_qfont.weight()==QFont::Bold?1:0).arg(m_stTextAttr->m_qfont.italic()?1:0).
            arg(m_stTextAttr->m_qfont.underline()?1:0 ).arg(iAlignment).arg(iMarginL).arg(iMarginR).
            arg(iMarginV);

        document()->lineCount();
        //Format:   Layer, Start,      End,        Style,  Name, MarginL, MarginR, MarginV, Effect, Text
        m_stTextAttr->m_qsEvent = QString(tr(
        "Dialogue:  0,     0:00:00.00, 0:00:01.94, %1,         , 0,       0,       0,           , {\\q2}%2")).
            arg((int)this).arg(toPlainText());
    }
}
#endif
