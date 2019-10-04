/****************************************************************************
** Meta object code from reading C++ file 'AVDemuxer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AVDemuxer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVDemuxer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVDemuxer_t {
    QByteArrayData data[14];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVDemuxer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVDemuxer_t qt_meta_stringdata_QtAV__AVDemuxer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QtAV::AVDemuxer"
QT_MOC_LITERAL(1, 16, 8), // "unloaded"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "userInterrupted"
QT_MOC_LITERAL(4, 42, 6), // "loaded"
QT_MOC_LITERAL(5, 49, 7), // "started"
QT_MOC_LITERAL(6, 57, 8), // "finished"
QT_MOC_LITERAL(7, 66, 5), // "error"
QT_MOC_LITERAL(8, 72, 13), // "QtAV::AVError"
QT_MOC_LITERAL(9, 86, 1), // "e"
QT_MOC_LITERAL(10, 88, 18), // "mediaStatusChanged"
QT_MOC_LITERAL(11, 107, 17), // "QtAV::MediaStatus"
QT_MOC_LITERAL(12, 125, 6), // "status"
QT_MOC_LITERAL(13, 132, 15) // "seekableChanged"

    },
    "QtAV::AVDemuxer\0unloaded\0\0userInterrupted\0"
    "loaded\0started\0finished\0error\0"
    "QtAV::AVError\0e\0mediaStatusChanged\0"
    "QtAV::MediaStatus\0status\0seekableChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVDemuxer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    0,   57,    2, 0x06 /* Public */,
       6,    0,   58,    2, 0x06 /* Public */,
       7,    1,   59,    2, 0x06 /* Public */,
      10,    1,   62,    2, 0x06 /* Public */,
      13,    0,   65,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

       0        // eod
};

void QtAV::AVDemuxer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVDemuxer *_t = static_cast<AVDemuxer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->unloaded(); break;
        case 1: _t->userInterrupted(); break;
        case 2: _t->loaded(); break;
        case 3: _t->started(); break;
        case 4: _t->finished(); break;
        case 5: _t->error((*reinterpret_cast< const QtAV::AVError(*)>(_a[1]))); break;
        case 6: _t->mediaStatusChanged((*reinterpret_cast< QtAV::MediaStatus(*)>(_a[1]))); break;
        case 7: _t->seekableChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AVError >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::unloaded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::userInterrupted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::loaded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::started)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::finished)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)(const QtAV::AVError & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::error)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)(QtAV::MediaStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::mediaStatusChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AVDemuxer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVDemuxer::seekableChanged)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AVDemuxer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AVDemuxer.data,
      qt_meta_data_QtAV__AVDemuxer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVDemuxer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVDemuxer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVDemuxer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AVDemuxer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void QtAV::AVDemuxer::unloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::AVDemuxer::userInterrupted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::AVDemuxer::loaded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AVDemuxer::started()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::AVDemuxer::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void QtAV::AVDemuxer::error(const QtAV::AVError & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QtAV::AVDemuxer::mediaStatusChanged(QtAV::MediaStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QtAV::AVDemuxer::seekableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
