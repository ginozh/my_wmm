/****************************************************************************
** Meta object code from reading C++ file 'VideoEncoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/VideoEncoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoEncoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoEncoder_t {
    QByteArrayData data[11];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoEncoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoEncoder_t qt_meta_stringdata_QtAV__VideoEncoder = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QtAV::VideoEncoder"
QT_MOC_LITERAL(1, 19, 12), // "widthChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "heightChanged"
QT_MOC_LITERAL(4, 47, 16), // "frameRateChanged"
QT_MOC_LITERAL(5, 64, 18), // "pixelFormatChanged"
QT_MOC_LITERAL(6, 83, 5), // "width"
QT_MOC_LITERAL(7, 89, 6), // "height"
QT_MOC_LITERAL(8, 96, 9), // "frameRate"
QT_MOC_LITERAL(9, 106, 11), // "pixelFormat"
QT_MOC_LITERAL(10, 118, 30) // "QtAV::VideoFormat::PixelFormat"

    },
    "QtAV::VideoEncoder\0widthChanged\0\0"
    "heightChanged\0frameRateChanged\0"
    "pixelFormatChanged\0width\0height\0"
    "frameRate\0pixelFormat\0"
    "QtAV::VideoFormat::PixelFormat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoEncoder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   38, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    0,   36,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::Int, 0x00495103,
       7, QMetaType::Int, 0x00495103,
       8, QMetaType::QReal, 0x00495103,
       9, 0x80000000 | 10, 0x0049510b,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,

       0        // eod
};

void QtAV::VideoEncoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoEncoder *_t = static_cast<VideoEncoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->widthChanged(); break;
        case 1: _t->heightChanged(); break;
        case 2: _t->frameRateChanged(); break;
        case 3: _t->pixelFormatChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncoder::widthChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncoder::heightChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncoder::frameRateChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncoder::pixelFormatChanged)) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::VideoFormat::PixelFormat >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        VideoEncoder *_t = static_cast<VideoEncoder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->width(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->height(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = _t->frameRate(); break;
        case 3: *reinterpret_cast< QtAV::VideoFormat::PixelFormat*>(_v) = _t->pixelFormat(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        VideoEncoder *_t = static_cast<VideoEncoder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setWidth(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setHeight(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setFrameRate(*reinterpret_cast< qreal*>(_v)); break;
        case 3: _t->setPixelFormat(*reinterpret_cast< QtAV::VideoFormat::PixelFormat*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::VideoEncoder::staticMetaObject = {
    { &AVEncoder::staticMetaObject, qt_meta_stringdata_QtAV__VideoEncoder.data,
      qt_meta_data_QtAV__VideoEncoder,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoEncoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoEncoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoEncoder.stringdata0))
        return static_cast<void*>(this);
    return AVEncoder::qt_metacast(_clname);
}

int QtAV::VideoEncoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AVEncoder::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::VideoEncoder::widthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::VideoEncoder::heightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::VideoEncoder::frameRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::VideoEncoder::pixelFormatChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
