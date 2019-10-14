/****************************************************************************
** Meta object code from reading C++ file 'AVEncoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AVEncoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVEncoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVEncoder_t {
    QByteArrayData data[15];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVEncoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVEncoder_t qt_meta_stringdata_QtAV__AVEncoder = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QtAV::AVEncoder"
QT_MOC_LITERAL(1, 16, 5), // "error"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "QtAV::AVError"
QT_MOC_LITERAL(4, 37, 1), // "e"
QT_MOC_LITERAL(5, 39, 16), // "codecNameChanged"
QT_MOC_LITERAL(6, 56, 14), // "bitRateChanged"
QT_MOC_LITERAL(7, 71, 20), // "timestampModeChanged"
QT_MOC_LITERAL(8, 92, 13), // "TimestampMode"
QT_MOC_LITERAL(9, 106, 4), // "mode"
QT_MOC_LITERAL(10, 111, 7), // "bitRate"
QT_MOC_LITERAL(11, 119, 9), // "codecName"
QT_MOC_LITERAL(12, 129, 13), // "timestampMode"
QT_MOC_LITERAL(13, 143, 18), // "TimestampMonotonic"
QT_MOC_LITERAL(14, 162, 13) // "TimestampCopy"

    },
    "QtAV::AVEncoder\0error\0\0QtAV::AVError\0"
    "e\0codecNameChanged\0bitRateChanged\0"
    "timestampModeChanged\0TimestampMode\0"
    "mode\0bitRate\0codecName\0timestampMode\0"
    "TimestampMonotonic\0TimestampCopy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVEncoder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       3,   42, // properties
       1,   54, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,
       6,    0,   38,    2, 0x06 /* Public */,
       7,    1,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

 // properties: name, type, flags
      10, QMetaType::Int, 0x00495103,
      11, QMetaType::QString, 0x00495103,
      12, 0x80000000 | 8, 0x0049510b,

 // properties: notify_signal_id
       2,
       1,
       3,

 // enums: name, flags, count, data
       8, 0x0,    2,   58,

 // enum data: key, value
      13, uint(QtAV::AVEncoder::TimestampMonotonic),
      14, uint(QtAV::AVEncoder::TimestampCopy),

       0        // eod
};

void QtAV::AVEncoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVEncoder *_t = static_cast<AVEncoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< const QtAV::AVError(*)>(_a[1]))); break;
        case 1: _t->codecNameChanged(); break;
        case 2: _t->bitRateChanged(); break;
        case 3: _t->timestampModeChanged((*reinterpret_cast< TimestampMode(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AVError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVEncoder::*_t)(const QtAV::AVError & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVEncoder::error)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVEncoder::codecNameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVEncoder::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVEncoder::bitRateChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVEncoder::*_t)(TimestampMode );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVEncoder::timestampModeChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AVEncoder *_t = static_cast<AVEncoder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->bitRate(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->codecName(); break;
        case 2: *reinterpret_cast< TimestampMode*>(_v) = _t->timestampMode(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AVEncoder *_t = static_cast<AVEncoder *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBitRate(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setCodecName(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setTimestampMode(*reinterpret_cast< TimestampMode*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::AVEncoder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AVEncoder.data,
      qt_meta_data_QtAV__AVEncoder,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVEncoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVEncoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVEncoder.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AVEncoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::AVEncoder::error(const QtAV::AVError & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AVEncoder::codecNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::AVEncoder::bitRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AVEncoder::timestampModeChanged(TimestampMode _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
