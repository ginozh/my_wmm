/****************************************************************************
** Meta object code from reading C++ file 'AVFilterSubtitle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../examples/player/filters/AVFilterSubtitle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVFilterSubtitle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AVFilterSubtitle_t {
    QByteArrayData data[14];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AVFilterSubtitle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AVFilterSubtitle_t qt_meta_stringdata_AVFilterSubtitle = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AVFilterSubtitle"
QT_MOC_LITERAL(1, 17, 6), // "loaded"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "loadError"
QT_MOC_LITERAL(4, 35, 11), // "fileChanged"
QT_MOC_LITERAL(5, 47, 4), // "path"
QT_MOC_LITERAL(6, 52, 15), // "autoLoadChanged"
QT_MOC_LITERAL(7, 68, 5), // "value"
QT_MOC_LITERAL(8, 74, 11), // "setAutoLoad"
QT_MOC_LITERAL(9, 86, 14), // "findAndSetFile"
QT_MOC_LITERAL(10, 101, 13), // "onPlayerStart"
QT_MOC_LITERAL(11, 115, 15), // "onStatusChanged"
QT_MOC_LITERAL(12, 131, 8), // "autoLoad"
QT_MOC_LITERAL(13, 140, 4) // "file"

    },
    "AVFilterSubtitle\0loaded\0\0loadError\0"
    "fileChanged\0path\0autoLoadChanged\0value\0"
    "setAutoLoad\0findAndSetFile\0onPlayerStart\0"
    "onStatusChanged\0autoLoad\0file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AVFilterSubtitle[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       2,   70, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,
       6,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   62,    2, 0x0a /* Public */,
       9,    1,   65,    2, 0x0a /* Public */,
      10,    0,   68,    2, 0x0a /* Public */,
      11,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::Bool, 0x00495103,
      13, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       3,
       2,

       0        // eod
};

void AVFilterSubtitle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVFilterSubtitle *_t = static_cast<AVFilterSubtitle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loaded(); break;
        case 1: _t->loadError(); break;
        case 2: _t->fileChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->autoLoadChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setAutoLoad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->findAndSetFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onPlayerStart(); break;
        case 7: _t->onStatusChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVFilterSubtitle::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVFilterSubtitle::loaded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVFilterSubtitle::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVFilterSubtitle::loadError)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVFilterSubtitle::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVFilterSubtitle::fileChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVFilterSubtitle::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVFilterSubtitle::autoLoadChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AVFilterSubtitle *_t = static_cast<AVFilterSubtitle *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->autoLoad(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->file(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AVFilterSubtitle *_t = static_cast<AVFilterSubtitle *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setAutoLoad(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setFile(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject AVFilterSubtitle::staticMetaObject = {
    { &LibAVFilterVideo::staticMetaObject, qt_meta_stringdata_AVFilterSubtitle.data,
      qt_meta_data_AVFilterSubtitle,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AVFilterSubtitle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AVFilterSubtitle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AVFilterSubtitle.stringdata0))
        return static_cast<void*>(this);
    return LibAVFilterVideo::qt_metacast(_clname);
}

int AVFilterSubtitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LibAVFilterVideo::qt_metacall(_c, _id, _a);
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
void AVFilterSubtitle::loaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AVFilterSubtitle::loadError()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AVFilterSubtitle::fileChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AVFilterSubtitle::autoLoadChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
