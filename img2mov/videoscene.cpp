#include "videoscene.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    //myItemType = DiagramItem::Step;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::white;
    myLineColor = Qt::black;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{/*
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }*/
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{/*
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
    */
}
//! [3]
void DiagramScene::setTextAttr(void* element, stTextAttr *textAttr)
{
    if(!textAttr)
        return;
    myFont = textAttr->m_qfont;
    if(element && m_mapText.contains(element))
    {
        DiagramTextItem *textItem = m_mapText[element];
        textItem->setFont(myFont);
        textItem->setTextAttr(textAttr);
    }
}

void DiagramScene::setFont(void* element, const QFont &font)
{
    myFont = font;
    if(element && m_mapText.contains(element))
    {
        DiagramTextItem *textItem = m_mapText[element];
        textItem->setFont(myFont);
    }
}
//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    //QMessageBox::information(NULL, "info", QString(tr("DiagramScene::setFont")));
    if (isItemChange(DiagramTextItem::Type)) 
    {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}
#if 0
void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}
#endif
//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
#if 0
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);
#endif
    //QMessageBox::information(NULL, "info", QString(tr("DiagramScene::editorLostFocus")));

    // 失去焦点则马上生成带文字的视频
    //
    // 1, 判断是否修改过 umcomplete
    //
    // 2, 如果修改过内容则生成文字视频
    emit updatedTextSignal(item->textAttr(), item->toPlainText());
#if 0
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
#endif
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
#if 0
    DiagramItem *item;
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;
//! [6] //! [7]
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
//! [7] //! [8]
        case InsertText:
#endif

#if 0
            if(!textItem)
            {
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            }
#endif

#if 0
//! [8] //! [9]
    default:
        ;
    }
#endif
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
#if 0
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
#endif
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]
void DiagramScene::createText(void* element)
{
    if(!m_mapText.contains(element))
    {
        DiagramTextItem *textItem;
        textItem = new DiagramTextItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        //textItem->setPos(mouseEvent->scenePos());
        //QString qsContent = QString(tr("element: %1")).arg((size_t)element);
        //textItem->setPlainText(qsContent);
        //textItem->setPos(width()/2-qsContent.length()/2,height()*3/5);
        textItem->setPos(width()/2,height()*3/5);
        textItem->hide();

        QTextBlockFormat format;
        format.setAlignment(Qt::AlignCenter);
        QTextCursor cursor = textItem->textCursor();
        cursor.select(QTextCursor::Document);
        cursor.mergeBlockFormat(format);
        cursor.clearSelection();
        textItem->setTextCursor(cursor);

        emit textInserted(textItem);

        m_mapText.insert(element, textItem);
    }
}
void DiagramScene::displayVideoText(void* element, bool isDisplay)
{
    if(m_mapText.contains(element))
    {
        DiagramTextItem *textItem = m_mapText[element];
        if(isDisplay)
            textItem->show();
        else
            textItem->hide();
    }
}
void DiagramScene::activeVideoText(void* element, const QString& oritxt)
{
    if(m_mapText.contains(element))
    {
        //QMessageBox::information(NULL, "info", QString(tr("DiagramScene::activeVideoText")));
        DiagramTextItem *textItem = m_mapText[element];
        setFocusItem(textItem);
        textItem->setActive(true);
        textItem->setVisible(true);
        textItem->setFocus(Qt::MouseFocusReason);
        textItem->grabKeyboard();
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        if(textItem->toPlainText().isEmpty())
        {
            textItem->setPlainText(oritxt);
        }
    }
}
//void DiagramScene::setVideoText(void* element, QFont& qfont)
