/****************************************************************************
** Meta object code from reading C++ file 'EncodeFilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/EncodeFilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EncodeFilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AudioEncodeFilter_t {
    QByteArrayData data[14];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AudioEncodeFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AudioEncodeFilter_t qt_meta_stringdata_QtAV__AudioEncodeFilter = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QtAV::AudioEncodeFilter"
QT_MOC_LITERAL(1, 24, 8), // "finished"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "readyToEncode"
QT_MOC_LITERAL(4, 48, 12), // "frameEncoded"
QT_MOC_LITERAL(5, 61, 12), // "QtAV::Packet"
QT_MOC_LITERAL(6, 74, 6), // "packet"
QT_MOC_LITERAL(7, 81, 16), // "startTimeChanged"
QT_MOC_LITERAL(8, 98, 5), // "value"
QT_MOC_LITERAL(9, 104, 15), // "requestToEncode"
QT_MOC_LITERAL(10, 120, 16), // "QtAV::AudioFrame"
QT_MOC_LITERAL(11, 137, 5), // "frame"
QT_MOC_LITERAL(12, 143, 6), // "finish"
QT_MOC_LITERAL(13, 150, 6) // "encode"

    },
    "QtAV::AudioEncodeFilter\0finished\0\0"
    "readyToEncode\0frameEncoded\0QtAV::Packet\0"
    "packet\0startTimeChanged\0value\0"
    "requestToEncode\0QtAV::AudioFrame\0frame\0"
    "finish\0encode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AudioEncodeFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,
       7,    1,   59,    2, 0x06 /* Public */,
       9,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   65,    2, 0x0a /* Public */,
      13,    1,   66,    2, 0x09 /* Protected */,
      13,    0,   69,    2, 0x29 /* Protected | MethodCloned */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void QtAV::AudioEncodeFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioEncodeFilter *_t = static_cast<AudioEncodeFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->readyToEncode(); break;
        case 2: _t->frameEncoded((*reinterpret_cast< const QtAV::Packet(*)>(_a[1]))); break;
        case 3: _t->startTimeChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: _t->requestToEncode((*reinterpret_cast< const QtAV::AudioFrame(*)>(_a[1]))); break;
        case 5: _t->finish(); break;
        case 6: _t->encode((*reinterpret_cast< const QtAV::AudioFrame(*)>(_a[1]))); break;
        case 7: _t->encode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AudioFrame >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AudioFrame >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AudioEncodeFilter::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioEncodeFilter::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AudioEncodeFilter::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioEncodeFilter::readyToEncode)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AudioEncodeFilter::*_t)(const QtAV::Packet & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioEncodeFilter::frameEncoded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AudioEncodeFilter::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioEncodeFilter::startTimeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AudioEncodeFilter::*_t)(const QtAV::AudioFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioEncodeFilter::requestToEncode)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QtAV::AudioEncodeFilter::staticMetaObject = {
    { &AudioFilter::staticMetaObject, qt_meta_stringdata_QtAV__AudioEncodeFilter.data,
      qt_meta_data_QtAV__AudioEncodeFilter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AudioEncodeFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AudioEncodeFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AudioEncodeFilter.stringdata0))
        return static_cast<void*>(this);
    return AudioFilter::qt_metacast(_clname);
}

int QtAV::AudioEncodeFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AudioFilter::qt_metacall(_c, _id, _a);
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
void QtAV::AudioEncodeFilter::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::AudioEncodeFilter::readyToEncode()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::AudioEncodeFilter::frameEncoded(const QtAV::Packet & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtAV::AudioEncodeFilter::startTimeChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtAV::AudioEncodeFilter::requestToEncode(const QtAV::AudioFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_QtAV__VideoEncodeFilter_t {
    QByteArrayData data[14];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoEncodeFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoEncodeFilter_t qt_meta_stringdata_QtAV__VideoEncodeFilter = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QtAV::VideoEncodeFilter"
QT_MOC_LITERAL(1, 24, 8), // "finished"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "readyToEncode"
QT_MOC_LITERAL(4, 48, 12), // "frameEncoded"
QT_MOC_LITERAL(5, 61, 12), // "QtAV::Packet"
QT_MOC_LITERAL(6, 74, 6), // "packet"
QT_MOC_LITERAL(7, 81, 16), // "startTimeChanged"
QT_MOC_LITERAL(8, 98, 5), // "value"
QT_MOC_LITERAL(9, 104, 15), // "requestToEncode"
QT_MOC_LITERAL(10, 120, 16), // "QtAV::VideoFrame"
QT_MOC_LITERAL(11, 137, 5), // "frame"
QT_MOC_LITERAL(12, 143, 6), // "finish"
QT_MOC_LITERAL(13, 150, 6) // "encode"

    },
    "QtAV::VideoEncodeFilter\0finished\0\0"
    "readyToEncode\0frameEncoded\0QtAV::Packet\0"
    "packet\0startTimeChanged\0value\0"
    "requestToEncode\0QtAV::VideoFrame\0frame\0"
    "finish\0encode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoEncodeFilter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,
       7,    1,   59,    2, 0x06 /* Public */,
       9,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   65,    2, 0x0a /* Public */,
      13,    1,   66,    2, 0x09 /* Protected */,
      13,    0,   69,    2, 0x29 /* Protected | MethodCloned */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::LongLong,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,

       0        // eod
};

void QtAV::VideoEncodeFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoEncodeFilter *_t = static_cast<VideoEncodeFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->readyToEncode(); break;
        case 2: _t->frameEncoded((*reinterpret_cast< const QtAV::Packet(*)>(_a[1]))); break;
        case 3: _t->startTimeChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 4: _t->requestToEncode((*reinterpret_cast< const QtAV::VideoFrame(*)>(_a[1]))); break;
        case 5: _t->finish(); break;
        case 6: _t->encode((*reinterpret_cast< const QtAV::VideoFrame(*)>(_a[1]))); break;
        case 7: _t->encode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::VideoFrame >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::VideoFrame >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoEncodeFilter::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncodeFilter::finished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoEncodeFilter::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncodeFilter::readyToEncode)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoEncodeFilter::*_t)(const QtAV::Packet & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncodeFilter::frameEncoded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoEncodeFilter::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncodeFilter::startTimeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoEncodeFilter::*_t)(const QtAV::VideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEncodeFilter::requestToEncode)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QtAV::VideoEncodeFilter::staticMetaObject = {
    { &VideoFilter::staticMetaObject, qt_meta_stringdata_QtAV__VideoEncodeFilter.data,
      qt_meta_data_QtAV__VideoEncodeFilter,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoEncodeFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoEncodeFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoEncodeFilter.stringdata0))
        return static_cast<void*>(this);
    return VideoFilter::qt_metacast(_clname);
}

int QtAV::VideoEncodeFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VideoFilter::qt_metacall(_c, _id, _a);
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
void QtAV::VideoEncodeFilter::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::VideoEncodeFilter::readyToEncode()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::VideoEncodeFilter::frameEncoded(const QtAV::Packet & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtAV::VideoEncodeFilter::startTimeChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtAV::VideoEncodeFilter::requestToEncode(const QtAV::VideoFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
