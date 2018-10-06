#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
//#include "Camera.h"

class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    GraphicsTextItem();
    QRectF boundingRect() const;
    void hitFrame(QGraphicsSceneHoverEvent *event);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        //emit lostFocus(this);
        QGraphicsTextItem::focusOutEvent(event);
    }
    //! [2]

    //! [5]
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        if (textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
//! [5]
};
class ClickableTextItem: public QGraphicsTextItem
{
    Q_OBJECT
public:
    ClickableTextItem( QGraphicsItem* pParent = 0 );
signals:
    void Clicked( void );
protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent* pEvent );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent* pEvent );
    void mousePressEvent( QGraphicsSceneMouseEvent* pEvent );
};

class GraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene( QObject* pParent = 0 );
    //void SetCamera( Camera* pCamera );
signals:
    void SwitchShader( const QString& shaderFileName );
    void SetLightPos( const QVector3D& pos );
protected:
    void mousePressEvent( QGraphicsSceneMouseEvent* pEvent );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* pEvent );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent* pEvent );
    void wheelEvent( QGraphicsSceneWheelEvent* pEvent );
private slots:
    void Feedback( void );
private:
    // 鼠标事件需要
    QPointF         m_LastPos;

    //Camera*         m_pCamera;
};

#endif // GRAPHICSSCENE_H
