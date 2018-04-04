#ifndef PLAYERPRIVATE_H
#define PLAYERPRIVATE_H

#include <QThread>
class GLDisplayWidget;
class PlayerPrivate : public QThread
{
    Q_OBJECT
public:
    PlayerPrivate(QObject *parent = 0);
    ~PlayerPrivate();
    void setGLWidget(GLDisplayWidget* glw);
    void stopPlayback();
protected:
    void run();
public:
    bool m_bstart=false;
    bool abort=false;
private:
    GLDisplayWidget* glwidget=NULL;
};

#endif
