#include "videoscene.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QList>
#include <QGraphicsView>
#include <QKeyEvent>

//! [0]
GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    curtextItem = 0;
}
//! [0]

void GraphicsScene::setTextAttr(void* element, stTextAttr *textAttr)
{
    if(!textAttr)
        return;
    myFont = textAttr->m_qfont;
    if(element && m_mapText.contains(element))
    {
        GraphicsTextItem *textItem = m_mapText[element];
        textItem->setFont(myFont);
        textItem->setTextAttr(textAttr);
    }
}

//! [5]
void GraphicsScene::editorLostFocus(GraphicsTextItem *item)
{
#if 0
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
#endif
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::editorLostFocus")));

    // 失去焦点则马上生成带文字的视频
    //
    // 1, 判断是否修改过 umcomplete
    //
    // 2, 如果修改过内容则生成文字视频
    if(item->getChanged())
    {
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::editorLostFocus")));
        emit updatedTextSignal(item->textAttr(), item->toPlainText());
    }
#if 0
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
#endif
    curtextItem = 0;
}
//! [5]



void GraphicsScene::createText(void* element)
{
    if(!m_mapText.contains(element))
    {
        GraphicsTextItem *textItem;
        textItem = new GraphicsTextItem();
        curtextItem = textItem;
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(GraphicsTextItem*)),
                this, SLOT(editorLostFocus(GraphicsTextItem*)));
#if 0
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
#endif
        addItem(textItem);
        textItem->setDefaultTextColor(Qt::white);
        //textItem->setPos(mouseEvent->scenePos());
        //QString qsContent = QString(tr("element: %1")).arg((size_t)element);
        //textItem->setPlainText(qsContent);
        //textItem->setPos(width()/2-qsContent.length()/2,height()*3/5);
        textItem->setPos(width()/2,height()*3/5);
        //textItem->setFocus();

        m_mapText.insert(element, textItem);
    }
}
void GraphicsScene::displayVideoText(void* element, bool isDisplay)
{
    if(m_mapText.contains(element))
    {
        GraphicsTextItem *textItem = m_mapText[element];
        if(isDisplay)
        {
            textItem->show();
            textItem->setEnabled(true);
            //disable other
            for( auto key: m_mapText.keys() )
            {
                if(key != element)
                {
                    GraphicsTextItem *tmptextItem = m_mapText.value( key );
                    tmptextItem->setVisible(false);
                    tmptextItem->setEnabled(false);
                }

            }
        }
        else
        {
            //textItem->hide();
            textItem->setVisible(false);
            textItem->setEnabled(false);
        }
    }
}
void GraphicsScene::activeVideoText(void* element, const QString& oritxt)
{
    if(m_mapText.contains(element))
    {
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::activeVideoText")));
        GraphicsTextItem *textItem = m_mapText[element];
        curtextItem = textItem;
#if 1
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setFocus(Qt::OtherFocusReason);
        displayVideoText(element, true);
        //textItem->setVisible(true);
        //textItem->setEnabled(true);
#if 0
        setFocusItem(textItem);
        textItem->setActive(true);
        textItem->setVisible(true);
        textItem->setSelected(true);
        textItem->setEnabled(true);
#endif
        //textItem->grabKeyboard();
        //textItem->update();
        //textItem->setSelected(true);
        //emit textItem->selectedChange(textItem);
#else
        textItem->setFocus(Qt::OtherFocusReason);
        textItem->setActive(true);
#endif

        textItem->setFirstTextPosWH(oritxt);
#if 0
        if(textItem->toPlainText().isEmpty())
        {
            QFont font;
            QFontMetrics fontMetrics(font);
            int textWidth = fontMetrics.width(oritxt);
            textItem->setTextWidth(textWidth+10);
            textItem->setPos(width()/2-textItem->textWidth()/2,height()*3/5);

            textItem->setPlainText(oritxt);
        }
#endif
    }
}
#if 0
//void GraphicsScene::setVideoText(void* element, QFont& qfont)
bool GraphicsScene::eventFilter( QObject *o, QEvent *e ) 
{
	if ( e->type() == QEvent::KeyPress ) {
		// 对于键被按下进行特殊处理
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
		return true; // 吃掉了这个事件
		//return QGraphicsScene::eventFilter( o, e );
	} else {
		// 标准事件处理
		return QGraphicsScene::eventFilter( o, e );
	}
}
#endif
void GraphicsScene::keyPressEvent(QKeyEvent *keyEvent) 
{
    //m_player->Scene()->setFocus();
    //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::keyPressEvent curtextItem: %1")).arg((int)curtextItem));
#if 1
    if(curtextItem)
    {
        GraphicsTextItem *textItem = curtextItem;
        textItem->setFocus(Qt::OtherFocusReason);
        bool b = sendEvent(curtextItem, keyEvent);
        //QMessageBox::information(NULL, "info", QString(tr("GraphicsScene::eventFilter key: %1 ok: %2")).arg(k->text()).arg(b));
    }
    else
#endif
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}
