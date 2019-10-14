/****************************************************************************
** Meta object code from reading C++ file 'AudioOutput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AudioOutput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioOutput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AudioOutput_t {
    QByteArrayData data[20];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AudioOutput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AudioOutput_t qt_meta_stringdata_QtAV__AudioOutput = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QtAV::AudioOutput"
QT_MOC_LITERAL(1, 18, 13), // "volumeChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "muteChanged"
QT_MOC_LITERAL(4, 45, 21), // "deviceFeaturesChanged"
QT_MOC_LITERAL(5, 67, 15), // "backendsChanged"
QT_MOC_LITERAL(6, 83, 12), // "reportVolume"
QT_MOC_LITERAL(7, 96, 5), // "value"
QT_MOC_LITERAL(8, 102, 10), // "reportMute"
QT_MOC_LITERAL(9, 113, 6), // "volume"
QT_MOC_LITERAL(10, 120, 4), // "mute"
QT_MOC_LITERAL(11, 125, 14), // "deviceFeatures"
QT_MOC_LITERAL(12, 140, 14), // "DeviceFeatures"
QT_MOC_LITERAL(13, 155, 8), // "backends"
QT_MOC_LITERAL(14, 164, 13), // "DeviceFeature"
QT_MOC_LITERAL(15, 178, 9), // "NoFeature"
QT_MOC_LITERAL(16, 188, 9), // "SetVolume"
QT_MOC_LITERAL(17, 198, 7), // "SetMute"
QT_MOC_LITERAL(18, 206, 13), // "SetSampleRate"
QT_MOC_LITERAL(19, 220, 8) // "SetSpeed"

    },
    "QtAV::AudioOutput\0volumeChanged\0\0"
    "muteChanged\0deviceFeaturesChanged\0"
    "backendsChanged\0reportVolume\0value\0"
    "reportMute\0volume\0mute\0deviceFeatures\0"
    "DeviceFeatures\0backends\0DeviceFeature\0"
    "NoFeature\0SetVolume\0SetMute\0SetSampleRate\0"
    "SetSpeed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AudioOutput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   58, // properties
       2,   74, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    0,   50,    2, 0x06 /* Public */,
       5,    0,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   52,    2, 0x08 /* Private */,
       8,    1,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QReal,    7,
    QMetaType::Void, QMetaType::Bool,    7,

 // properties: name, type, flags
       9, QMetaType::QReal, 0x00495103,
      10, QMetaType::Bool, 0x00495103,
      11, 0x80000000 | 12, 0x0049510b,
      13, QMetaType::QStringList, 0x00495103,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,

 // enums: name, flags, count, data
      14, 0x0,    5,   82,
      12, 0x1,    5,   92,

 // enum data: key, value
      15, uint(QtAV::AudioOutput::NoFeature),
      16, uint(QtAV::AudioOutput::SetVolume),
      17, uint(QtAV::AudioOutput::SetMute),
      18, uint(QtAV::AudioOutput::SetSampleRate),
      19, uint(QtAV::AudioOutput::SetSpeed),
      15, uint(QtAV::AudioOutput::NoFeature),
      16, uint(QtAV::AudioOutput::SetVolume),
      17, uint(QtAV::AudioOutput::SetMute),
      18, uint(QtAV::AudioOutput::SetSampleRate),
      19, uint(QtAV::AudioOutput::SetSpeed),

       0        // eod
};

void QtAV::AudioOutput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioOutput *_t = static_cast<AudioOutput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->volumeChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->muteChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->deviceFeaturesChanged(); break;
        case 3: _t->backendsChanged(); break;
        case 4: _t->reportVolume((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 5: _t->reportMute((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AudioOutput::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioOutput::volumeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AudioOutput::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioOutput::muteChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AudioOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioOutput::deviceFeaturesChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AudioOutput::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioOutput::backendsChanged)) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AudioOutput *_t = static_cast<AudioOutput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->volume(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isMute(); break;
        case 2: *reinterpret_cast<int*>(_v) = QFlag(_t->deviceFeatures()); break;
        case 3: *reinterpret_cast< QStringList*>(_v) = _t->backends(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AudioOutput *_t = static_cast<AudioOutput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setVolume(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setMute(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setDeviceFeatures(QFlag(*reinterpret_cast<int*>(_v))); break;
        case 3: _t->setBackends(*reinterpret_cast< QStringList*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::AudioOutput::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AudioOutput.data,
      qt_meta_data_QtAV__AudioOutput,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AudioOutput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AudioOutput::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AudioOutput.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "AVOutput"))
        return static_cast< AVOutput*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AudioOutput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::AudioOutput::volumeChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AudioOutput::muteChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtAV::AudioOutput::deviceFeaturesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AudioOutput::backendsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
