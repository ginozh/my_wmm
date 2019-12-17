#ifndef HWDECODERFACTORY_H
#define HWDECODERFACTORY_H

#include <QQmlEngine>
#include "hwdecoder.h"
class GLWidget;
class HWDecoderFactory
{
public:
    static HWDecoder *createDecoder(QObject * parent = nullptr, GLWidget* glwidget = nullptr);

};

#endif // HWDECODERFACTORY_H
