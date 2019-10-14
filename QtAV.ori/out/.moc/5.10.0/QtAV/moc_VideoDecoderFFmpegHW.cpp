/****************************************************************************
** Meta object code from reading C++ file 'VideoDecoderFFmpegHW.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/codec/video/VideoDecoderFFmpegHW.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoDecoderFFmpegHW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW_t {
    QByteArrayData data[10];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW_t qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW = {
    {
QT_MOC_LITERAL(0, 0, 26), // "QtAV::VideoDecoderFFmpegHW"
QT_MOC_LITERAL(1, 27, 15), // "copyModeChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 14), // "threadsChanged"
QT_MOC_LITERAL(4, 59, 7), // "threads"
QT_MOC_LITERAL(5, 67, 8), // "copyMode"
QT_MOC_LITERAL(6, 76, 8), // "CopyMode"
QT_MOC_LITERAL(7, 85, 8), // "ZeroCopy"
QT_MOC_LITERAL(8, 94, 13), // "OptimizedCopy"
QT_MOC_LITERAL(9, 108, 11) // "GenericCopy"

    },
    "QtAV::VideoDecoderFFmpegHW\0copyModeChanged\0"
    "\0threadsChanged\0threads\0copyMode\0"
    "CopyMode\0ZeroCopy\0OptimizedCopy\0"
    "GenericCopy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoDecoderFFmpegHW[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       2,   26, // properties
       1,   34, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::Int, 0x00495103,
       5, 0x80000000 | 6, 0x0049510b,

 // properties: notify_signal_id
       1,
       0,

 // enums: name, flags, count, data
       6, 0x0,    3,   38,

 // enum data: key, value
       7, uint(QtAV::VideoDecoderFFmpegHW::ZeroCopy),
       8, uint(QtAV::VideoDecoderFFmpegHW::OptimizedCopy),
       9, uint(QtAV::VideoDecoderFFmpegHW::GenericCopy),

       0        // eod
};

void QtAV::VideoDecoderFFmpegHW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoDecoderFFmpegHW *_t = static_cast<VideoDecoderFFmpegHW *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->copyModeChanged(); break;
        case 1: _t->threadsChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoDecoderFFmpegHW::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoDecoderFFmpegHW::copyModeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoDecoderFFmpegHW::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoDecoderFFmpegHW::threadsChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        VideoDecoderFFmpegHW *_t = static_cast<VideoDecoderFFmpegHW *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->threads(); break;
        case 1: *reinterpret_cast< CopyMode*>(_v) = _t->copyMode(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        VideoDecoderFFmpegHW *_t = static_cast<VideoDecoderFFmpegHW *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setThreads(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setCopyMode(*reinterpret_cast< CopyMode*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject QtAV::VideoDecoderFFmpegHW::staticMetaObject = {
    { &VideoDecoderFFmpegBase::staticMetaObject, qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW.data,
      qt_meta_data_QtAV__VideoDecoderFFmpegHW,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoDecoderFFmpegHW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoDecoderFFmpegHW::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoDecoderFFmpegHW.stringdata0))
        return static_cast<void*>(this);
    return VideoDecoderFFmpegBase::qt_metacast(_clname);
}

int QtAV::VideoDecoderFFmpegHW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VideoDecoderFFmpegBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::VideoDecoderFFmpegHW::copyModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::VideoDecoderFFmpegHW::threadsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
