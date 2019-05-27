#ifndef CLIP_H
#define CLIP_H

#include <QList>
#include <QColor>
#include "project/effect.h"
class OpenGLWidget;
class Clip : public QObject
{
	Q_OBJECT
public:
    Clip();
signals:
	void start_create_effect_ui(Clip* c);
public:
    void createEffect();
	QList<Effect*> effects;
    OpenGLWidget* m_glwidget=NULL;
};

#endif
