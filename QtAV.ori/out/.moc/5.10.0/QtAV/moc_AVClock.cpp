/****************************************************************************
** Meta object code from reading C++ file 'AVClock.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AVClock.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVClock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVClock_t {
    QByteArrayData data[12];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVClock_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVClock_t qt_meta_stringdata_QtAV__AVClock = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QtAV::AVClock"
QT_MOC_LITERAL(1, 14, 6), // "paused"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "resumed"
QT_MOC_LITERAL(4, 30, 7), // "started"
QT_MOC_LITERAL(5, 38, 8), // "resetted"
QT_MOC_LITERAL(6, 47, 5), // "start"
QT_MOC_LITERAL(7, 53, 5), // "pause"
QT_MOC_LITERAL(8, 59, 1), // "p"
QT_MOC_LITERAL(9, 61, 5), // "reset"
QT_MOC_LITERAL(10, 67, 22), // "restartCorrectionTimer"
QT_MOC_LITERAL(11, 90, 19) // "stopCorrectionTimer"

    },
    "QtAV::AVClock\0paused\0\0resumed\0started\0"
    "resetted\0start\0pause\0p\0reset\0"
    "restartCorrectionTimer\0stopCorrectionTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVClock[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       1,    0,   67,    2, 0x06 /* Public */,
       3,    0,   68,    2, 0x06 /* Public */,
       4,    0,   69,    2, 0x06 /* Public */,
       5,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   71,    2, 0x0a /* Public */,
       7,    1,   72,    2, 0x0a /* Public */,
       9,    0,   75,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x08 /* Private */,
      11,    0,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtAV::AVClock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVClock *_t = static_cast<AVClock *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->paused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->paused(); break;
        case 2: _t->resumed(); break;
        case 3: _t->started(); break;
        case 4: _t->resetted(); break;
        case 5: _t->start(); break;
        case 6: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->reset(); break;
        case 8: _t->restartCorrectionTimer(); break;
        case 9: _t->stopCorrectionTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVClock::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVClock::paused)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVClock::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVClock::paused)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVClock::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVClock::resumed)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVClock::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVClock::started)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AVClock::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVClock::resetted)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AVClock::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AVClock.data,
      qt_meta_data_QtAV__AVClock,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVClock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVClock::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVClock.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AVClock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QtAV::AVClock::paused(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AVClock::paused()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::AVClock::resumed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AVClock::started()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::AVClock::resetted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
