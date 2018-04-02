#ifndef PLAYERPRIVATE_H
#define PLAYERPRIVATE_H

#include <QThread>
class GLWidget;
class PlayerPrivate : public QThread
{
    Q_OBJECT
public:
    PlayerPrivate(QObject *parent = 0);
    ~PlayerPrivate();
    void setGLWidget(GLWidget* glw);
protected:
    void run();
public:
    bool m_bstart=false;
private:
    GLWidget* glwidget=NULL;
};

#endif
