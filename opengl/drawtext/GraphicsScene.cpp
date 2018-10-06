#include <QDebug>
#include <QtOpenGL>
#include <QApplication>
#include <QGraphicsSceneHoverEvent>
#include "GraphicsScene.h"
//#include "ShaderOptionDialog.h"
//#include "Feedback/FeedbackDialog.h"

GraphicsTextItem::GraphicsTextItem()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setAcceptDrops(true);
    setAcceptHoverEvents(true);


    QTextDocument *tdocument =  document();
    tdocument->setModified(true);
    QTextOption option = tdocument->defaultTextOption();
    option.setAlignment(Qt::AlignHCenter);
    tdocument->setDefaultTextOption(option);
    //setTextWidth(tdocument->idealWidth());
    setTextWidth(400);

}
QRectF GraphicsTextItem::boundingRect() const
{
#if 0
    qreal penWidth = 1;

    return QRectF(-15, -50, 30, 50);
    return QRectF(- penWidth / 2,  - penWidth / 2,
           penWidth,  penWidth);
    qreal halfpw = 10;
    QRectF rect = QGraphicsTextItem::boundingRect();
    rect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    return rect;
#endif
    return QGraphicsTextItem::boundingRect();
}
void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#if 0
    //painter->drawRoundedRect(-10, -10, 20, 20, 5, 5);
    //QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
    //o->state |= QStyle::State_HasFocus|QStyle::State_Selected;
    //o->state |= QStyle::State_UpArrow;
    //QGraphicsTextItem::paint(painter, o, widget);
    QFontMetrics fm(font());

    QRect boxRect = fm.boundingRect(boundingRect().toRect(), Qt::AlignHCenter ,toPlainText());
    //QRectF boxRect = QRectF(0,0,40,15);
    QPen testPen = QPen(Qt::red);
    //painter->setPen(QPen(drawingColor,2));
    //painter->drawRect(boxRect);
    painter->setPen(testPen);
    painter->drawText(boxRect,Qt::AlignCenter,toPlainText());
#endif
    QPainterPath path;
    //path.moveTo(20, 80);
    //path.lineTo(20, 30);
    //path.cubicTo(80, 0, 50, 50, 80, 80);
#if 0
    // 只是改变字体border
    QFontMetrics fm(font());
    QString text=toPlainText();
    QRect curRt = fm.boundingRect(boundingRect().toRect(), Qt::AlignHCenter ,text);
    path.addText(curRt.left(),curRt.top()+fm.ascent(), font(), text); 

    QColor outlineColor(Qt::red);
    QPen pen(outlineColor);
    pen.setWidthF(1.5);
    painter->setPen(pen);
    painter->drawPath(path);
#endif
    QGraphicsTextItem::paint(painter, option, widget);
}
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
/*---------------------------------------------------------------------------*/
ClickableTextItem::ClickableTextItem( QGraphicsItem* pParent ):
    QGraphicsTextItem( pParent )
{

}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::hoverEnterEvent( QGraphicsSceneHoverEvent* pEvent )
{
    QApplication::setOverrideCursor( Qt::PointingHandCursor );
    QGraphicsTextItem::hoverEnterEvent( pEvent );
}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::hoverLeaveEvent( QGraphicsSceneHoverEvent* pEvent )
{
    QApplication::restoreOverrideCursor( );
    QGraphicsTextItem::hoverLeaveEvent( pEvent );
}
/*---------------------------------------------------------------------------*/
void ClickableTextItem::mousePressEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsTextItem::mousePressEvent( pEvent );
    emit Clicked( );
}
/*---------------------------------------------------------------------------*/
GraphicsScene::GraphicsScene( QObject* pParent ):
    QGraphicsScene( pParent )//, m_pCamera( Q_NULLPTR )
{
#if 0
    ClickableTextItem* pTextItem = new ClickableTextItem( Q_NULLPTR );
    pTextItem->setPos( 10.0, 10.0 );
    pTextItem->setHtml( tr( "<font color=white>"
                            "Made By Jiangcaiyang<br>"
                            "Created in September<br>"
                            "Click for feedback."
                            "</font>" ) );
    connect( pTextItem, SIGNAL( Clicked( ) ),
             this, SLOT( Feedback( ) ) );
    addItem( pTextItem );
#endif
    GraphicsTextItem* textItem = new GraphicsTextItem();
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setPlainText("Qt Everywhere!");
    textItem->setDefaultTextColor(Qt::red);
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setZValue(1000.0);
    textItem->setPos(50, 300);


    addItem(textItem);
#if 0
    ShaderOptionDialog* pDialog = new ShaderOptionDialog;
    connect( pDialog, SIGNAL( SwitchShader( const QString& ) ),
             this, SIGNAL( SwitchShader( const QString& ) ) );
    connect( pDialog, SIGNAL( SetLightPos( const QVector3D& ) ),
             this, SIGNAL( SetLightPos( const QVector3D& ) ) );

    QGraphicsProxyWidget* pProxy = addWidget( pDialog, Qt::Window | Qt::WindowTitleHint );
    pProxy->setPos( 100, 200 );
#endif
}
/*---------------------------------------------------------------------------*/
#if 0
void GraphicsScene::SetCamera( Camera *pCamera )
{
    m_pCamera = pCamera;
}
#endif
/*---------------------------------------------------------------------------*/
void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mousePressEvent( pEvent );
    if ( pEvent->isAccepted( ) ) return;
    m_LastPos = pEvent->scenePos( );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:

        break;
    case Qt::MiddleButton:
    default: break;
    }
    pEvent->accept( );
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::mouseMoveEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mouseMoveEvent( pEvent );
#if 0
    if ( pEvent->isAccepted( ) ) return;
    QVector2D delta( pEvent->scenePos( ) - m_LastPos );
    if ( pEvent->buttons( ) & Qt::LeftButton ||
         pEvent->buttons( ) & Qt::RightButton )
    {
        m_pCamera->SetRotateH( -delta.y( ), true );
        m_pCamera->SetRotateY( delta.x( ), true );
    }
    else if ( pEvent->buttons( ) & Qt::MiddleButton )
    {
        m_pCamera->SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ), true );
    }
    pEvent->accept( );
#endif
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* pEvent )
{
    QGraphicsScene::mouseReleaseEvent( pEvent );
#if 0
    if ( pEvent->isAccepted( ) ) return;
    QVector2D delta( pEvent->scenePos( ) - m_LastPos );
    switch ( pEvent->button( ) )
    {
    case Qt::LeftButton:
    case Qt::RightButton:
        m_pCamera->SetRotateH( -delta.y( ) );
        m_pCamera->SetRotateY( delta.x( ) );
        break;
    case Qt::MiddleButton:
        m_pCamera->SetPos( QVector3D( -delta.x( ) / 5, delta.y( ) / 5, 0 ) );
        break;
    default: break;
    }
    pEvent->accept( );
#endif
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::wheelEvent( QGraphicsSceneWheelEvent* pEvent )
{
    QGraphicsScene::wheelEvent( pEvent );
#if 0
    if ( pEvent->isAccepted( ) ) return;
    m_pCamera->ZoomIn( pEvent->delta( ) / 50 );
    pEvent->accept( );
#endif
}
/*---------------------------------------------------------------------------*/
void GraphicsScene::Feedback( void )
{
#if 0
    FeedbackDialog feedback( tr( "Light for shader" ),
                             Q_NULLPTR );
    feedback.exec( );
#endif
}
