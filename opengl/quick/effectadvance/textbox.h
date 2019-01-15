#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QtQuick>

//! [0]
class TextBox : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool rightAligned READ isRightAligned WRITE setRightAligned NOTIFY rightAlignedChanged)

    public:
        TextBox(QQuickItem *parent = 0);
        void paint(QPainter *painter);

        bool isRightAligned();
        void setRightAligned(bool rightAligned);

    private:
        bool rightAligned;

    signals:
        void rightAlignedChanged();
};
//! [0]

#endif
