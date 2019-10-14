/****************************************************************************
** Meta object code from reading C++ file 'AVTranscoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AVTranscoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVTranscoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVTranscoder_t {
    QByteArrayData data[22];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVTranscoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVTranscoder_t qt_meta_stringdata_QtAV__AVTranscoder = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QtAV::AVTranscoder"
QT_MOC_LITERAL(1, 19, 17), // "videoFrameEncoded"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 9), // "timestamp"
QT_MOC_LITERAL(4, 48, 17), // "audioFrameEncoded"
QT_MOC_LITERAL(5, 66, 7), // "started"
QT_MOC_LITERAL(6, 74, 7), // "stopped"
QT_MOC_LITERAL(7, 82, 6), // "paused"
QT_MOC_LITERAL(8, 89, 5), // "value"
QT_MOC_LITERAL(9, 95, 16), // "startTimeChanged"
QT_MOC_LITERAL(10, 112, 2), // "ms"
QT_MOC_LITERAL(11, 115, 12), // "asyncChanged"
QT_MOC_LITERAL(12, 128, 5), // "start"
QT_MOC_LITERAL(13, 134, 4), // "stop"
QT_MOC_LITERAL(14, 139, 5), // "pause"
QT_MOC_LITERAL(15, 145, 15), // "onSourceStarted"
QT_MOC_LITERAL(16, 161, 12), // "prepareMuxer"
QT_MOC_LITERAL(17, 174, 10), // "writeAudio"
QT_MOC_LITERAL(18, 185, 12), // "QtAV::Packet"
QT_MOC_LITERAL(19, 198, 6), // "packet"
QT_MOC_LITERAL(20, 205, 10), // "writeVideo"
QT_MOC_LITERAL(21, 216, 9) // "tryFinish"

    },
    "QtAV::AVTranscoder\0videoFrameEncoded\0"
    "\0timestamp\0audioFrameEncoded\0started\0"
    "stopped\0paused\0value\0startTimeChanged\0"
    "ms\0asyncChanged\0start\0stop\0pause\0"
    "onSourceStarted\0prepareMuxer\0writeAudio\0"
    "QtAV::Packet\0packet\0writeVideo\0tryFinish"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVTranscoder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,
       5,    0,   95,    2, 0x06 /* Public */,
       6,    0,   96,    2, 0x06 /* Public */,
       7,    1,   97,    2, 0x06 /* Public */,
       9,    1,  100,    2, 0x06 /* Public */,
      11,    0,  103,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  104,    2, 0x0a /* Public */,
      13,    0,  105,    2, 0x0a /* Public */,
      14,    1,  106,    2, 0x0a /* Public */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    1,  111,    2, 0x08 /* Private */,
      20,    1,  114,    2, 0x08 /* Private */,
      21,    0,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::LongLong,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,

       0        // eod
};

void QtAV::AVTranscoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVTranscoder *_t = static_cast<AVTranscoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->videoFrameEncoded((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->audioFrameEncoded((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 2: _t->started(); break;
        case 3: _t->stopped(); break;
        case 4: _t->paused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->startTimeChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: _t->asyncChanged(); break;
        case 7: _t->start(); break;
        case 8: _t->stop(); break;
        case 9: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->onSourceStarted(); break;
        case 11: _t->prepareMuxer(); break;
        case 12: _t->writeAudio((*reinterpret_cast< const QtAV::Packet(*)>(_a[1]))); break;
        case 13: _t->writeVideo((*reinterpret_cast< const QtAV::Packet(*)>(_a[1]))); break;
        case 14: _t->tryFinish(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVTranscoder::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::videoFrameEncoded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::audioFrameEncoded)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::started)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::stopped)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::paused)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::startTimeChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AVTranscoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVTranscoder::asyncChanged)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AVTranscoder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AVTranscoder.data,
      qt_meta_data_QtAV__AVTranscoder,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVTranscoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVTranscoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVTranscoder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AVTranscoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void QtAV::AVTranscoder::videoFrameEncoded(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AVTranscoder::audioFrameEncoded(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtAV::AVTranscoder::started()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AVTranscoder::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::AVTranscoder::paused(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtAV::AVTranscoder::startTimeChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtAV::AVTranscoder::asyncChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
