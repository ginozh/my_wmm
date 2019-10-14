/****************************************************************************
** Meta object code from reading C++ file 'AVDemuxThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AVDemuxThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVDemuxThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVDemuxThread_t {
    QByteArrayData data[18];
    char stringdata0[280];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVDemuxThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVDemuxThread_t qt_meta_stringdata_QtAV__AVDemuxThread = {
    {
QT_MOC_LITERAL(0, 0, 19), // "QtAV::AVDemuxThread"
QT_MOC_LITERAL(1, 20, 17), // "requestClockPause"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 5), // "value"
QT_MOC_LITERAL(4, 45, 18), // "mediaStatusChanged"
QT_MOC_LITERAL(5, 64, 17), // "QtAV::MediaStatus"
QT_MOC_LITERAL(6, 82, 21), // "bufferProgressChanged"
QT_MOC_LITERAL(7, 104, 12), // "seekFinished"
QT_MOC_LITERAL(8, 117, 9), // "timestamp"
QT_MOC_LITERAL(9, 127, 12), // "stepFinished"
QT_MOC_LITERAL(10, 140, 26), // "internalSubtitlePacketRead"
QT_MOC_LITERAL(11, 167, 5), // "index"
QT_MOC_LITERAL(12, 173, 12), // "QtAV::Packet"
QT_MOC_LITERAL(13, 186, 6), // "packet"
QT_MOC_LITERAL(14, 193, 19), // "seekOnPauseFinished"
QT_MOC_LITERAL(15, 213, 27), // "frameDeliveredOnStepForward"
QT_MOC_LITERAL(16, 241, 23), // "eofDecodedOnStepForward"
QT_MOC_LITERAL(17, 265, 14) // "onAVThreadQuit"

    },
    "QtAV::AVDemuxThread\0requestClockPause\0"
    "\0value\0mediaStatusChanged\0QtAV::MediaStatus\0"
    "bufferProgressChanged\0seekFinished\0"
    "timestamp\0stepFinished\0"
    "internalSubtitlePacketRead\0index\0"
    "QtAV::Packet\0packet\0seekOnPauseFinished\0"
    "frameDeliveredOnStepForward\0"
    "eofDecodedOnStepForward\0onAVThreadQuit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVDemuxThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    1,   70,    2, 0x06 /* Public */,
       7,    1,   73,    2, 0x06 /* Public */,
       9,    0,   76,    2, 0x06 /* Public */,
      10,    2,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   82,    2, 0x08 /* Private */,
      15,    0,   83,    2, 0x08 /* Private */,
      16,    0,   84,    2, 0x08 /* Private */,
      17,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 12,   11,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtAV::AVDemuxThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVDemuxThread *_t = static_cast<AVDemuxThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestClockPause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->mediaStatusChanged((*reinterpret_cast< QtAV::MediaStatus(*)>(_a[1]))); break;
        case 2: _t->bufferProgressChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 3: _t->seekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: _t->stepFinished(); break;
        case 5: _t->internalSubtitlePacketRead((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QtAV::Packet(*)>(_a[2]))); break;
        case 6: _t->seekOnPauseFinished(); break;
        case 7: _t->frameDeliveredOnStepForward(); break;
        case 8: _t->eofDecodedOnStepForward(); break;
        case 9: _t->onAVThreadQuit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVDemuxThread::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::requestClockPause)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVDemuxThread::*_t)(QtAV::MediaStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::mediaStatusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVDemuxThread::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::bufferProgressChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVDemuxThread::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::seekFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AVDemuxThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::stepFinished)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AVDemuxThread::*_t)(int , const QtAV::Packet & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxThread::internalSubtitlePacketRead)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AVDemuxThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QtAV__AVDemuxThread.data,
      qt_meta_data_QtAV__AVDemuxThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVDemuxThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVDemuxThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVDemuxThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int QtAV::AVDemuxThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QtAV::AVDemuxThread::requestClockPause(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AVDemuxThread::mediaStatusChanged(QtAV::MediaStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtAV::AVDemuxThread::bufferProgressChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtAV::AVDemuxThread::seekFinished(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtAV::AVDemuxThread::stepFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void QtAV::AVDemuxThread::internalSubtitlePacketRead(int _t1, const QtAV::Packet & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
