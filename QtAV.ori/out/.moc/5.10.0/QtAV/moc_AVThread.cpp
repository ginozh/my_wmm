/****************************************************************************
** Meta object code from reading C++ file 'AVThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AVThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVThread_t {
    QByteArrayData data[12];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVThread_t qt_meta_stringdata_QtAV__AVThread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "QtAV::AVThread"
QT_MOC_LITERAL(1, 15, 14), // "frameDelivered"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "seekFinished"
QT_MOC_LITERAL(4, 44, 9), // "timestamp"
QT_MOC_LITERAL(5, 54, 10), // "eofDecoded"
QT_MOC_LITERAL(6, 65, 4), // "stop"
QT_MOC_LITERAL(7, 70, 5), // "pause"
QT_MOC_LITERAL(8, 76, 1), // "p"
QT_MOC_LITERAL(9, 78, 12), // "nextAndPause"
QT_MOC_LITERAL(10, 91, 9), // "onStarted"
QT_MOC_LITERAL(11, 101, 10) // "onFinished"

    },
    "QtAV::AVThread\0frameDelivered\0\0"
    "seekFinished\0timestamp\0eofDecoded\0"
    "stop\0pause\0p\0nextAndPause\0onStarted\0"
    "onFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       5,    0,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   59,    2, 0x0a /* Public */,
       7,    1,   60,    2, 0x0a /* Public */,
       9,    0,   63,    2, 0x0a /* Public */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtAV::AVThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVThread *_t = static_cast<AVThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameDelivered(); break;
        case 1: _t->seekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: _t->eofDecoded(); break;
        case 3: _t->stop(); break;
        case 4: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->nextAndPause(); break;
        case 6: _t->onStarted(); break;
        case 7: _t->onFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVThread::frameDelivered)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVThread::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVThread::seekFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVThread::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVThread::eofDecoded)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AVThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QtAV__AVThread.data,
      qt_meta_data_QtAV__AVThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int QtAV::AVThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QtAV::AVThread::frameDelivered()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::AVThread::seekFinished(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtAV::AVThread::eofDecoded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
