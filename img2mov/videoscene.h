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
class Element;

//! [0]
class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QObject *parent = 0);
    //void setFont(void* element, const QFont &font);
    void setVideoTextAttr(Element* element, GlobalTextAttr* globalTextAttr);
    //void setTextAttr(void* element, stTextAttr *textAttr);
    GraphicsTextItem* getGraphicsTextItem(){return curtextItem;}
    QString createTotalAssInfo();

public slots:
    //void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(GraphicsTextItem *item);
    //void createVideoText(void*);
    void createVideoText(Element*);
    void displayVideoText(Element*, bool);
    void activeVideoText(Element* element, const QString&);

signals:
    //void updatedTextSignal(stTextAttr*, const QString&);
    void updatedElementsTextSignal(const QString&, const QString&);

protected:
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    //bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;

private:
    
    QMap<Element*, GraphicsTextItem*> m_mapText;
    GraphicsTextItem *curtextItem;

    QFont myFont;
    QColor myTextColor;
};
//! [0]

#endif // DIAGRAMSCENE_H
