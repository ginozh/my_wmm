#ifndef VIDEORECTITEM_H
#define VIDEORECTITEM_H
#include <QGraphicsRectItem>
#include <QCursor>

class GraphicsRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    enum PosInHost{NORTH_MIDDLE,NORTH_EAST,EAST_MIDDLE,SOUTH_EAST,SOUTH_MIDDLE,SOUTH_WEST,WEST_MIDDLE,NORTH_WEST};
    enum { Type = UserType + 1 };
    GraphicsRectItem(PosInHost pos, QGraphicsItem *parent);
    GraphicsRectItem(qreal len, PosInHost pos, QGraphicsItem *parent);
    ~GraphicsRectItem();
    int type() const {return Type;};
    void setInHost(PosInHost pos){m_posInHost = pos;};
    PosInHost getInHost(){return m_posInHost;};
    void locateInHost();
protected:
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
private:
    PosInHost m_posInHost;
    qreal m_wsize;
};

#endif // VIDEORECTITEM_H
