#ifndef PLAYERPRIVATE_H
#define PLAYERPRIVATE_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
//#include "ReaderBase.h"
#include <memory>
class GLDisplayWidget;
class GLHiddenWidget;

class PlayerPrivate : public QThread
{
    Q_OBJECT
public:
    PlayerPrivate(QObject *parent = 0);
    ~PlayerPrivate();
    void setGLWidget(GLDisplayWidget* glw);
    void startPlayback();
    void stopPlayback();
protected:
    void run();
private:
    GLDisplayWidget* m_glwidget=NULL;
    GLHiddenWidget* m_glhiddenwidget=NULL;
    bool abort;
public:
    int ready=0;
};

#endif
