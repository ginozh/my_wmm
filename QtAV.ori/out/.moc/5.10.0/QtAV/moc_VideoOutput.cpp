/****************************************************************************
** Meta object code from reading C++ file 'VideoOutput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/VideoOutput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoOutput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoOutput_t {
    QByteArrayData data[28];
    char stringdata0[442];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoOutput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoOutput_t qt_meta_stringdata_QtAV__VideoOutput = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QtAV::VideoOutput"
QT_MOC_LITERAL(1, 18, 24), // "sourceAspectRatioChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 5), // "value"
QT_MOC_LITERAL(4, 50, 23), // "regionOfInterestChanged"
QT_MOC_LITERAL(5, 74, 21), // "outAspectRatioChanged"
QT_MOC_LITERAL(6, 96, 25), // "outAspectRatioModeChanged"
QT_MOC_LITERAL(7, 122, 17), // "brightnessChanged"
QT_MOC_LITERAL(8, 140, 15), // "contrastChanged"
QT_MOC_LITERAL(9, 156, 10), // "hueChanged"
QT_MOC_LITERAL(10, 167, 17), // "saturationChanged"
QT_MOC_LITERAL(11, 185, 22), // "backgroundColorChanged"
QT_MOC_LITERAL(12, 208, 18), // "orientationChanged"
QT_MOC_LITERAL(13, 227, 16), // "videoRectChanged"
QT_MOC_LITERAL(14, 244, 21), // "videoFrameSizeChanged"
QT_MOC_LITERAL(15, 266, 10), // "brightness"
QT_MOC_LITERAL(16, 277, 8), // "contrast"
QT_MOC_LITERAL(17, 286, 3), // "hue"
QT_MOC_LITERAL(18, 290, 10), // "saturation"
QT_MOC_LITERAL(19, 301, 15), // "backgroundColor"
QT_MOC_LITERAL(20, 317, 16), // "regionOfInterest"
QT_MOC_LITERAL(21, 334, 17), // "sourceAspectRatio"
QT_MOC_LITERAL(22, 352, 14), // "outAspectRatio"
QT_MOC_LITERAL(23, 367, 18), // "outAspectRatioMode"
QT_MOC_LITERAL(24, 386, 18), // "OutAspectRatioMode"
QT_MOC_LITERAL(25, 405, 11), // "orientation"
QT_MOC_LITERAL(26, 417, 9), // "videoRect"
QT_MOC_LITERAL(27, 427, 14) // "videoFrameSize"

    },
    "QtAV::VideoOutput\0sourceAspectRatioChanged\0"
    "\0value\0regionOfInterestChanged\0"
    "outAspectRatioChanged\0outAspectRatioModeChanged\0"
    "brightnessChanged\0contrastChanged\0"
    "hueChanged\0saturationChanged\0"
    "backgroundColorChanged\0orientationChanged\0"
    "videoRectChanged\0videoFrameSizeChanged\0"
    "brightness\0contrast\0hue\0saturation\0"
    "backgroundColor\0regionOfInterest\0"
    "sourceAspectRatio\0outAspectRatio\0"
    "outAspectRatioMode\0OutAspectRatioMode\0"
    "orientation\0videoRect\0videoFrameSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoOutput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
      12,   96, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    0,   77,    2, 0x06 /* Public */,
       5,    0,   78,    2, 0x06 /* Public */,
       6,    0,   79,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,
       8,    1,   83,    2, 0x06 /* Public */,
       9,    1,   86,    2, 0x06 /* Public */,
      10,    1,   89,    2, 0x06 /* Public */,
      11,    0,   92,    2, 0x06 /* Public */,
      12,    0,   93,    2, 0x06 /* Public */,
      13,    0,   94,    2, 0x06 /* Public */,
      14,    0,   95,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      15, QMetaType::QReal, 0x00495103,
      16, QMetaType::QReal, 0x00495103,
      17, QMetaType::QReal, 0x00495103,
      18, QMetaType::QReal, 0x00495103,
      19, QMetaType::QColor, 0x00495103,
      20, QMetaType::QRectF, 0x00495103,
      21, QMetaType::QReal, 0x00495001,
      22, QMetaType::QReal, 0x00495103,
      23, 0x80000000 | 24, 0x0049510b,
      25, QMetaType::Int, 0x00495103,
      26, QMetaType::QRect, 0x00495001,
      27, QMetaType::QSize, 0x00495001,

 // properties: notify_signal_id
       4,
       5,
       6,
       7,
       8,
       1,
       0,
       2,
       3,
       9,
      10,
      11,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

void QtAV::VideoOutput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoOutput *_t = static_cast<VideoOutput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sourceAspectRatioChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->regionOfInterestChanged(); break;
        case 2: _t->outAspectRatioChanged(); break;
        case 3: _t->outAspectRatioModeChanged(); break;
        case 4: _t->brightnessChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 5: _t->contrastChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 6: _t->hueChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 7: _t->saturationChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 8: _t->backgroundColorChanged(); break;
        case 9: _t->orientationChanged(); break;
        case 10: _t->videoRectChanged(); break;
        case 11: _t->videoFrameSizeChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::sourceAspectRatioChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::regionOfInterestChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::outAspectRatioChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::outAspectRatioModeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::brightnessChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::contrastChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::hueChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::saturationChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::backgroundColorChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::orientationChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::videoRectChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (VideoOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOutput::videoFrameSizeChanged)) {
                *result = 11;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        VideoOutput *_t = static_cast<VideoOutput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->brightness(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = _t->contrast(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->hue(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->saturation(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = _t->backgroundColor(); break;
        case 5: *reinterpret_cast< QRectF*>(_v) = _t->regionOfInterest(); break;
        case 6: *reinterpret_cast< qreal*>(_v) = _t->sourceAspectRatio(); break;
        case 7: *reinterpret_cast< qreal*>(_v) = _t->outAspectRatio(); break;
        case 8: *reinterpret_cast< OutAspectRatioMode*>(_v) = _t->outAspectRatioMode(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->orientation(); break;
        case 10: *reinterpret_cast< QRect*>(_v) = _t->videoRect(); break;
        case 11: *reinterpret_cast< QSize*>(_v) = _t->videoFrameSize(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        VideoOutput *_t = static_cast<VideoOutput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBrightness(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setContrast(*reinterpret_cast< qreal*>(_v)); break;
        case 2: _t->setHue(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setSaturation(*reinterpret_cast< qreal*>(_v)); break;
        case 4: _t->setBackgroundColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: _t->setRegionOfInterest(*reinterpret_cast< QRectF*>(_v)); break;
        case 7: _t->setOutAspectRatio(*reinterpret_cast< qreal*>(_v)); break;
        case 8: _t->setOutAspectRatioMode(*reinterpret_cast< OutAspectRatioMode*>(_v)); break;
        case 9: _t->setOrientation(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::VideoOutput::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__VideoOutput.data,
      qt_meta_data_QtAV__VideoOutput,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoOutput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoOutput::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoOutput.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "VideoRenderer"))
        return static_cast< VideoRenderer*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::VideoOutput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 12;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 12;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::VideoOutput::sourceAspectRatioChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::VideoOutput::regionOfInterestChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::VideoOutput::outAspectRatioChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::VideoOutput::outAspectRatioModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::VideoOutput::brightnessChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtAV::VideoOutput::contrastChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtAV::VideoOutput::hueChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QtAV::VideoOutput::saturationChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QtAV::VideoOutput::backgroundColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void QtAV::VideoOutput::orientationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void QtAV::VideoOutput::videoRectChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void QtAV::VideoOutput::videoFrameSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
