/****************************************************************************
** Meta object code from reading C++ file 'VideoFrameExtractor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/VideoFrameExtractor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoFrameExtractor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoFrameExtractor_t {
    QByteArrayData data[22];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoFrameExtractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoFrameExtractor_t qt_meta_stringdata_QtAV__VideoFrameExtractor = {
    {
QT_MOC_LITERAL(0, 0, 25), // "QtAV::VideoFrameExtractor"
QT_MOC_LITERAL(1, 26, 14), // "frameExtracted"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 16), // "QtAV::VideoFrame"
QT_MOC_LITERAL(4, 59, 5), // "frame"
QT_MOC_LITERAL(5, 65, 13), // "sourceChanged"
QT_MOC_LITERAL(6, 79, 12), // "asyncChanged"
QT_MOC_LITERAL(7, 92, 5), // "error"
QT_MOC_LITERAL(8, 98, 12), // "errorMessage"
QT_MOC_LITERAL(9, 111, 7), // "aborted"
QT_MOC_LITERAL(10, 119, 12), // "abortMessage"
QT_MOC_LITERAL(11, 132, 18), // "autoExtractChanged"
QT_MOC_LITERAL(12, 151, 15), // "positionChanged"
QT_MOC_LITERAL(13, 167, 16), // "precisionChanged"
QT_MOC_LITERAL(14, 184, 7), // "extract"
QT_MOC_LITERAL(15, 192, 15), // "extractInternal"
QT_MOC_LITERAL(16, 208, 3), // "pos"
QT_MOC_LITERAL(17, 212, 6), // "source"
QT_MOC_LITERAL(18, 219, 11), // "autoExtract"
QT_MOC_LITERAL(19, 231, 5), // "async"
QT_MOC_LITERAL(20, 237, 9), // "precision"
QT_MOC_LITERAL(21, 247, 8) // "position"

    },
    "QtAV::VideoFrameExtractor\0frameExtracted\0"
    "\0QtAV::VideoFrame\0frame\0sourceChanged\0"
    "asyncChanged\0error\0errorMessage\0aborted\0"
    "abortMessage\0autoExtractChanged\0"
    "positionChanged\0precisionChanged\0"
    "extract\0extractInternal\0pos\0source\0"
    "autoExtract\0async\0precision\0position"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoFrameExtractor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       5,   82, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    1,   69,    2, 0x06 /* Public */,
       9,    1,   72,    2, 0x06 /* Public */,
      11,    0,   75,    2, 0x06 /* Public */,
      12,    0,   76,    2, 0x06 /* Public */,
      13,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   78,    2, 0x0a /* Public */,
      15,    1,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   16,

 // properties: name, type, flags
      17, QMetaType::QString, 0x00495103,
      18, QMetaType::Bool, 0x00495103,
      19, QMetaType::Bool, 0x00495103,
      20, QMetaType::Int, 0x00495103,
      21, QMetaType::LongLong, 0x00495103,

 // properties: notify_signal_id
       1,
       5,
       2,
       7,
       6,

       0        // eod
};

void QtAV::VideoFrameExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoFrameExtractor *_t = static_cast<VideoFrameExtractor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameExtracted((*reinterpret_cast< const QtAV::VideoFrame(*)>(_a[1]))); break;
        case 1: _t->sourceChanged(); break;
        case 2: _t->asyncChanged(); break;
        case 3: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->aborted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->autoExtractChanged(); break;
        case 6: _t->positionChanged(); break;
        case 7: _t->precisionChanged(); break;
        case 8: _t->extract(); break;
        case 9: _t->extractInternal((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
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
            typedef void (VideoFrameExtractor::*_t)(const QtAV::VideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::frameExtracted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::sourceChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::asyncChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::error)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::aborted)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::autoExtractChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::positionChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (VideoFrameExtractor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameExtractor::precisionChanged)) {
                *result = 7;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        VideoFrameExtractor *_t = static_cast<VideoFrameExtractor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->source(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->autoExtract(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->async(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->precision(); break;
        case 4: *reinterpret_cast< qint64*>(_v) = _t->position(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        VideoFrameExtractor *_t = static_cast<VideoFrameExtractor *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSource(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setAutoExtract(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setAsync(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setPrecision(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setPosition(*reinterpret_cast< qint64*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::VideoFrameExtractor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__VideoFrameExtractor.data,
      qt_meta_data_QtAV__VideoFrameExtractor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoFrameExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoFrameExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoFrameExtractor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::VideoFrameExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::VideoFrameExtractor::frameExtracted(const QtAV::VideoFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::VideoFrameExtractor::sourceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::VideoFrameExtractor::asyncChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::VideoFrameExtractor::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QtAV::VideoFrameExtractor::aborted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtAV::VideoFrameExtractor::autoExtractChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void QtAV::VideoFrameExtractor::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void QtAV::VideoFrameExtractor::precisionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
