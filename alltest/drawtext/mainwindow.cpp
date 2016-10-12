#include <QtWidgets>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QLineEdit>

#include "mainwindow.h"
LineEdit::LineEdit(QWidget *parent, GraphicsScene* tscene)
    //: QLabel(parent)
    : QLineEdit(parent)
    , scene(tscene)
      //, m_idx(idx)
{
    setPlaceholderText("A[Enter text here]");
    setReadOnly(true);
    //connect(this, SIGNAL(selectTextSignal(int)), parentWidget(), SLOT(selectedText()));
    //connect(this, SIGNAL(selectTextSignal()), parentWidget(), SLOT(selectedText()));
}
void LineEdit::mousePressEvent(QMouseEvent *event)
{
    //emit selectTextSignal(m_idx);
    //QMessageBox::information(this, "info", QString(tr("textlabel")));
    emit selectedTextSignal(placeholderText());
    //QLabel::mouseDoubleClickEvent(event);
    //QLabel::mousePressEvent(event);
    //QLineEdit::mousePressEvent(event);
}
void LineEdit::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("LineEdit::keyPressEvent")));
    QCoreApplication::sendEvent(scene, keyEvent);
    //QLineEdit::keyPressEvent(keyEvent);
}
bool LineEdit::eventFilter( QObject *o, QEvent *e ) 
{
#if 1
	if ( e->type() == QEvent::KeyPress ) {
        return QObject::eventFilter(o, e);
		//return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QLineEdit::eventFilter( o, e );
	}
#endif
}
GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
#if 0
    GraphicsTextItem *textItem;
    textItem = new GraphicsTextItem();
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setPlainText("Qt Everywhere!");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setZValue(1000.0);
    textItem->setPos(50, 300);

    addItem(textItem);
    curtextItem = textItem;
#endif
}
#if 0
bool GraphicsScene::eventFilter( QObject *o, QEvent *e ) 
{
	if ( e->type() == QEvent::KeyPress ) {
		// 对于键被按下进行特殊处理
#if 0
		QKeyEvent *k = (QKeyEvent *)e;
        if(curtextItem)
        {
            GraphicsTextItem *textItem = curtextItem;
            textItem->setFocus(Qt::OtherFocusReason);
#if 0
            textItem->setActive(true);
            textItem->setVisible(true);
            textItem->setFocus(Qt::OtherFocusReason);
            textItem->grabKeyboard();
            //textItem->setSelected(true);
            //emit textItem->selectedChange(textItem);
#endif
            bool b = sendEvent(curtextItem, k);
            //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::eventFilter key: %1 ok: %2")).arg(k->text()).arg(b));
            qDebug( "iagramScene::eventFilter key press %d", k->key() );
        }
		//qDebug( "Ate key press %d", k->key() );
		//return true; // 吃掉了这个事件
#endif
		return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QGraphicsScene::eventFilter( o, e );
	}
}
#endif
#if 1
void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::keyPressEvent")));
#if 0
    if(curtextItem)
    {
        GraphicsTextItem *textItem = curtextItem;
        textItem->setFocus(Qt::OtherFocusReason);
        textItem->setCursor(Qt::IBeamCursor);
        QTextCursor textCursor=textItem->textCursor();
        textCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        textItem->setTextCursor(textCursor);
        bool b = sendEvent(curtextItem, keyEvent);
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::eventFilter key: %1 ok: %2")).arg(k->text()).arg(b));
    }
    else
#endif
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}
#endif
void GraphicsScene::activeVideoText(const QString& oritxt)
{
    GraphicsTextItem *textItem = curtextItem;
    textItem->setFocus(Qt::OtherFocusReason);
    if(textItem->toPlainText().isEmpty())
    {
        textItem->setPlainText(oritxt);
    }
}

GraphicsTextItem::GraphicsTextItem(){
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
MainWindow::MainWindow()
{
    scene = new GraphicsScene(this);
    QGraphicsView *graphicsView = new QGraphicsView(scene);

#if 0
    QGraphicsVideoItem* videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(500, 480));
    scene->addItem(videoItem);
#endif
    //installEventFilter( scene );
    textItem = new GraphicsTextItem();
    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    textItem->setFont(font);
    textItem->setPlainText("Qt Everywhere!");
    textItem->setDefaultTextColor(Qt::black);


#if 0
	QTextDocument* document = new QTextDocument;

	QTextCharFormat charFormat;
	charFormat.setFont(QFont("times", 24));

	QPen outlinePen = QPen (QColor(255, 0, 0), 1, Qt::SolidLine);
	charFormat.setTextOutline(outlinePen);
	//字体轮廓
	QTextCursor cursor = QTextCursor(document);
	cursor.insertText("Test", charFormat);
	textItem->setDocument(document);
	//textItem->setTextInteractionFlags(Qt::TextEditable);
#endif
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setZValue(1000.0);
    textItem->setPos(50, 300);


    scene->addItem(textItem);

    QWidget *tabAnimations = new QWidget;
    //tabAnimations->setMinimumHeight(200);
    //tabAnimations->setMaximumHeight(200);

    QHBoxLayout* hboxAnimations = new QHBoxLayout;
    hboxAnimations->addWidget(graphicsView);
    {
        QLabel *lbl = new QLabel(tr("Text duration:"));

        hboxAnimations->addWidget(lbl);
    }
    {
        QComboBox* m_durationTextCombo = new QComboBox();
        //m_durationTextCombo->setEditable(true);
        for (int i = 2; i < 30; i = i + 2)
            m_durationTextCombo->addItem(QString().setNum(i));

        hboxAnimations->addWidget(m_durationTextCombo);
    }
    {
        QToolButton *addPhotos = new QToolButton();
        addPhotos->setText("Add photos");
        hboxAnimations->addWidget(addPhotos);
        removeEventFilter(addPhotos);
    }
    {
        m_lineEdit =  new LineEdit(this,scene);
        hboxAnimations->addWidget(m_lineEdit);
        //removeEventFilter(m_lineEdit);
        //m_lineEdit->installEventFilter( this );
    }

    connect(m_lineEdit, SIGNAL(selectedTextSignal(const QString&)), scene,
            SLOT(activeVideoText(const QString&)));

    tabAnimations->setLayout(hboxAnimations);
    setCentralWidget(tabAnimations);
    setWindowTitle(tr("Character Map"));
}
//! [6]

#if 0
void MainWindow::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("MainWindow::keyPressEvent")));
    QMainWindow::keyPressEvent(keyEvent);
}
#endif
#if 0
bool MainWindow::eventFilter( QObject *o, QEvent *e ) 
{
	if ( e->type() == QEvent::KeyPress ) {
#if 0
        if (m_lineEdit==o)
        {
            //return true;
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
            //QMessageBox::information(NULL, "info", QString(tr("MainWindow::eventFilter")));
            QCoreApplication::sendEvent(scene, keyEvent);
            return true;
            //bool b = sendEvent(scene, keyEvent);
            //return QObject::eventFilter(scene, e);
        }
        else
        {
            return QObject::eventFilter(o, e);
        }
#endif 
		return QMainWindow::eventFilter( o, e );
		//return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QMainWindow::eventFilter( o, e );
	}
}
#endif
