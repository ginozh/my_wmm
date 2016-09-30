#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "videotextitem.h"

#include <QGraphicsScene>
#include <QMap>
#include "comm.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    explicit DiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);
    void setFont(void* element, const QFont &font);
    void setTextAttr(void* element, stTextAttr *textAttr);

public slots:
    void setMode(Mode mode);
    //void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(DiagramTextItem *item);
    void createText(void*);
    void displayVideoText(void*, bool);
    void activeVideoText(void* element, const QString&);

signals:
    //void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
    void updatedTextSignal(stTextAttr*, const QString&);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    bool isItemChange(int type);

    //DiagramItem::DiagramType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    DiagramTextItem *textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;

    
    QMap<void*, DiagramTextItem*> m_mapText;
};
//! [0]

#endif // DIAGRAMSCENE_H
