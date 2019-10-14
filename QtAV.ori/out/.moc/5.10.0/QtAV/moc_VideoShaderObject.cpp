/****************************************************************************
** Meta object code from reading C++ file 'VideoShaderObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/VideoShaderObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoShaderObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoShaderObject_t {
    QByteArrayData data[4];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoShaderObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoShaderObject_t qt_meta_stringdata_QtAV__VideoShaderObject = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QtAV::VideoShaderObject"
QT_MOC_LITERAL(1, 24, 15), // "propertyChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 2) // "id"

    },
    "QtAV::VideoShaderObject\0propertyChanged\0"
    "\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoShaderObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void QtAV::VideoShaderObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoShaderObject *_t = static_cast<VideoShaderObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->propertyChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QtAV::VideoShaderObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__VideoShaderObject.data,
      qt_meta_data_QtAV__VideoShaderObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoShaderObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoShaderObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoShaderObject.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "VideoShader"))
        return static_cast< VideoShader*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::VideoShaderObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_QtAV__DynamicShaderObject_t {
    QByteArrayData data[8];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__DynamicShaderObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__DynamicShaderObject_t qt_meta_stringdata_QtAV__DynamicShaderObject = {
    {
QT_MOC_LITERAL(0, 0, 25), // "QtAV::DynamicShaderObject"
QT_MOC_LITERAL(1, 26, 13), // "headerChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 13), // "sampleChanged"
QT_MOC_LITERAL(4, 55, 18), // "postProcessChanged"
QT_MOC_LITERAL(5, 74, 6), // "header"
QT_MOC_LITERAL(6, 81, 6), // "sample"
QT_MOC_LITERAL(7, 88, 11) // "postProcess"

    },
    "QtAV::DynamicShaderObject\0headerChanged\0"
    "\0sampleChanged\0postProcessChanged\0"
    "header\0sample\0postProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__DynamicShaderObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       3,   32, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       5, QMetaType::QString, 0x00495103,
       6, QMetaType::QString, 0x00495103,
       7, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void QtAV::DynamicShaderObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DynamicShaderObject *_t = static_cast<DynamicShaderObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->headerChanged(); break;
        case 1: _t->sampleChanged(); break;
        case 2: _t->postProcessChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (DynamicShaderObject::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DynamicShaderObject::headerChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DynamicShaderObject::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DynamicShaderObject::sampleChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DynamicShaderObject::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DynamicShaderObject::postProcessChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        DynamicShaderObject *_t = static_cast<DynamicShaderObject *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->header(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->sample(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->postProcess(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        DynamicShaderObject *_t = static_cast<DynamicShaderObject *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHeader(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setSample(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setPostProcess(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject QtAV::DynamicShaderObject::staticMetaObject = {
    { &VideoShaderObject::staticMetaObject, qt_meta_stringdata_QtAV__DynamicShaderObject.data,
      qt_meta_data_QtAV__DynamicShaderObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::DynamicShaderObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::DynamicShaderObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__DynamicShaderObject.stringdata0))
        return static_cast<void*>(this);
    return VideoShaderObject::qt_metacast(_clname);
}

int QtAV::DynamicShaderObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VideoShaderObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
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
void QtAV::DynamicShaderObject::headerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::DynamicShaderObject::sampleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::DynamicShaderObject::postProcessChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
