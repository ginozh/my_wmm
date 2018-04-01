#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
class MYSlider : public QSlider
{
    Q_OBJECT
public:
    MYSlider(QWidget *parent=0);
	MYSlider(Qt::Orientation orientation, QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};
#endif
