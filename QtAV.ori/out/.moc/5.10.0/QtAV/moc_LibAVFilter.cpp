/****************************************************************************
** Meta object code from reading C++ file 'LibAVFilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/LibAVFilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LibAVFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__LibAVFilterVideo_t {
    QByteArrayData data[5];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__LibAVFilterVideo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__LibAVFilterVideo_t qt_meta_stringdata_QtAV__LibAVFilterVideo = {
    {
QT_MOC_LITERAL(0, 0, 22), // "QtAV::LibAVFilterVideo"
QT_MOC_LITERAL(1, 23, 14), // "optionsChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "options"
QT_MOC_LITERAL(4, 47, 7) // "filters"

    },
    "QtAV::LibAVFilterVideo\0optionsChanged\0"
    "\0options\0filters"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__LibAVFilterVideo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495103,
       4, QMetaType::QStringList, 0x00095001,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

void QtAV::LibAVFilterVideo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LibAVFilterVideo *_t = static_cast<LibAVFilterVideo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->optionsChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (LibAVFilterVideo::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibAVFilterVideo::optionsChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LibAVFilterVideo *_t = static_cast<LibAVFilterVideo *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->options(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->filters(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LibAVFilterVideo *_t = static_cast<LibAVFilterVideo *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setOptions(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject QtAV::LibAVFilterVideo::staticMetaObject = {
    { &VideoFilter::staticMetaObject, qt_meta_stringdata_QtAV__LibAVFilterVideo.data,
      qt_meta_data_QtAV__LibAVFilterVideo,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::LibAVFilterVideo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::LibAVFilterVideo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__LibAVFilterVideo.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "LibAVFilter"))
        return static_cast< LibAVFilter*>(this);
    return VideoFilter::qt_metacast(_clname);
}

int QtAV::LibAVFilterVideo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VideoFilter::qt_metacall(_c, _id, _a);
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
void QtAV::LibAVFilterVideo::optionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_QtAV__LibAVFilterAudio_t {
    QByteArrayData data[5];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__LibAVFilterAudio_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__LibAVFilterAudio_t qt_meta_stringdata_QtAV__LibAVFilterAudio = {
    {
QT_MOC_LITERAL(0, 0, 22), // "QtAV::LibAVFilterAudio"
QT_MOC_LITERAL(1, 23, 14), // "optionsChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "options"
QT_MOC_LITERAL(4, 47, 7) // "filters"

    },
    "QtAV::LibAVFilterAudio\0optionsChanged\0"
    "\0options\0filters"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__LibAVFilterAudio[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   20, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495103,
       4, QMetaType::QStringList, 0x00095001,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

void QtAV::LibAVFilterAudio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LibAVFilterAudio *_t = static_cast<LibAVFilterAudio *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->optionsChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (LibAVFilterAudio::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LibAVFilterAudio::optionsChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LibAVFilterAudio *_t = static_cast<LibAVFilterAudio *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->options(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->filters(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LibAVFilterAudio *_t = static_cast<LibAVFilterAudio *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setOptions(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject QtAV::LibAVFilterAudio::staticMetaObject = {
    { &AudioFilter::staticMetaObject, qt_meta_stringdata_QtAV__LibAVFilterAudio.data,
      qt_meta_data_QtAV__LibAVFilterAudio,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::LibAVFilterAudio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::LibAVFilterAudio::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__LibAVFilterAudio.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "LibAVFilter"))
        return static_cast< LibAVFilter*>(this);
    return AudioFilter::qt_metacast(_clname);
}

int QtAV::LibAVFilterAudio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AudioFilter::qt_metacall(_c, _id, _a);
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
void QtAV::LibAVFilterAudio::optionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
