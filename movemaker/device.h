#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QFrame>

class Device : public QWidget
{
    Q_OBJECT

public:
    Device(QWidget *parent=NULL);
    ~Device();
private:
    //竖线
    QFrame *m_vecticalLine;
};

#endif // DEVICE_H
