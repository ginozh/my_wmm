#include "slider.h"
#include <QStyleOptionSlider>
#include <QStyle>
#include <QMouseEvent>

MYSlider::MYSlider(QWidget *parent)
    : QSlider(parent)
{
}
MYSlider::MYSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent)
{
}
void MYSlider::mousePressEvent(QMouseEvent *event)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton &&
            sr.contains(event->pos()) == false)
    {
        int newVal;
        if (orientation() == Qt::Vertical)
            newVal = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
        else
        {
            double halfHandleWidth = (0.5 * sr.width()) + 0.5; // Correct rounding
            int adaptedPosX = event->x();
            if ( adaptedPosX < halfHandleWidth )
                adaptedPosX = halfHandleWidth;
            if ( adaptedPosX > width() - halfHandleWidth )
                adaptedPosX = width() - halfHandleWidth;
            // get new dimensions accounting for slider handle width
            double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
            double normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth ;

            newVal = minimum() + ((maximum()-minimum()) * normalizedPosition);
            //newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();
        }
#if 0
        if (invertedAppearance() == true)
            setValue( maximum() - newVal );
        else
            setValue(newVal);
#else
        if (invertedAppearance() == true)
            newVal =  maximum() - newVal ;
        setValue(newVal);
        emit sliderMoved(newVal);
#endif

        event->accept();
    }
    QSlider::mousePressEvent(event);
}
