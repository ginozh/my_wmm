/****************************************************************************
** Meta object code from reading C++ file 'VideoCapture.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/VideoCapture.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoCapture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__VideoCapture_t {
    QByteArrayData data[27];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__VideoCapture_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__VideoCapture_t qt_meta_stringdata_QtAV__VideoCapture = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QtAV::VideoCapture"
QT_MOC_LITERAL(1, 19, 9), // "requested"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "frameAvailable"
QT_MOC_LITERAL(4, 45, 16), // "QtAV::VideoFrame"
QT_MOC_LITERAL(5, 62, 5), // "frame"
QT_MOC_LITERAL(6, 68, 13), // "imageCaptured"
QT_MOC_LITERAL(7, 82, 5), // "image"
QT_MOC_LITERAL(8, 88, 6), // "failed"
QT_MOC_LITERAL(9, 95, 5), // "saved"
QT_MOC_LITERAL(10, 101, 4), // "path"
QT_MOC_LITERAL(11, 106, 12), // "asyncChanged"
QT_MOC_LITERAL(12, 119, 15), // "autoSaveChanged"
QT_MOC_LITERAL(13, 135, 21), // "originalFormatChanged"
QT_MOC_LITERAL(14, 157, 17), // "saveFormatChanged"
QT_MOC_LITERAL(15, 175, 14), // "qualityChanged"
QT_MOC_LITERAL(16, 190, 18), // "captureNameChanged"
QT_MOC_LITERAL(17, 209, 17), // "captureDirChanged"
QT_MOC_LITERAL(18, 227, 7), // "capture"
QT_MOC_LITERAL(19, 235, 13), // "handleAppQuit"
QT_MOC_LITERAL(20, 249, 5), // "async"
QT_MOC_LITERAL(21, 255, 8), // "autoSave"
QT_MOC_LITERAL(22, 264, 14), // "originalFormat"
QT_MOC_LITERAL(23, 279, 10), // "saveFormat"
QT_MOC_LITERAL(24, 290, 7), // "quality"
QT_MOC_LITERAL(25, 298, 11), // "captureName"
QT_MOC_LITERAL(26, 310, 10) // "captureDir"

    },
    "QtAV::VideoCapture\0requested\0\0"
    "frameAvailable\0QtAV::VideoFrame\0frame\0"
    "imageCaptured\0image\0failed\0saved\0path\0"
    "asyncChanged\0autoSaveChanged\0"
    "originalFormatChanged\0saveFormatChanged\0"
    "qualityChanged\0captureNameChanged\0"
    "captureDirChanged\0capture\0handleAppQuit\0"
    "async\0autoSave\0originalFormat\0saveFormat\0"
    "quality\0captureName\0captureDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__VideoCapture[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       7,  104, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    1,   85,    2, 0x06 /* Public */,
       6,    1,   88,    2, 0x06 /* Public */,
       8,    0,   91,    2, 0x06 /* Public */,
       9,    1,   92,    2, 0x06 /* Public */,
      11,    0,   95,    2, 0x06 /* Public */,
      12,    0,   96,    2, 0x06 /* Public */,
      13,    0,   97,    2, 0x06 /* Public */,
      14,    0,   98,    2, 0x06 /* Public */,
      15,    0,   99,    2, 0x06 /* Public */,
      16,    0,  100,    2, 0x06 /* Public */,
      17,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  102,    2, 0x0a /* Public */,
      19,    0,  103,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      20, QMetaType::Bool, 0x00495103,
      21, QMetaType::Bool, 0x00495103,
      22, QMetaType::Bool, 0x00495103,
      23, QMetaType::QString, 0x00495103,
      24, QMetaType::Int, 0x00495103,
      25, QMetaType::QString, 0x00495103,
      26, QMetaType::QString, 0x00495103,

 // properties: notify_signal_id
       5,
       6,
       7,
       8,
       9,
      10,
      11,

       0        // eod
};

void QtAV::VideoCapture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoCapture *_t = static_cast<VideoCapture *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requested(); break;
        case 1: _t->frameAvailable((*reinterpret_cast< const QtAV::VideoFrame(*)>(_a[1]))); break;
        case 2: _t->imageCaptured((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 3: _t->failed(); break;
        case 4: _t->saved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->asyncChanged(); break;
        case 6: _t->autoSaveChanged(); break;
        case 7: _t->originalFormatChanged(); break;
        case 8: _t->saveFormatChanged(); break;
        case 9: _t->qualityChanged(); break;
        case 10: _t->captureNameChanged(); break;
        case 11: _t->captureDirChanged(); break;
        case 12: _t->capture(); break;
        case 13: _t->handleAppQuit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
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
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::requested)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)(const QtAV::VideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::frameAvailable)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)(const QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::imageCaptured)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::failed)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::saved)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::asyncChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::autoSaveChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::originalFormatChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::saveFormatChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::qualityChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::captureNameChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (VideoCapture::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCapture::captureDirChanged)) {
                *result = 11;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        VideoCapture *_t = static_cast<VideoCapture *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isAsync(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->autoSave(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isOriginalFormat(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->saveFormat(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->quality(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->captureName(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->captureDir(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        VideoCapture *_t = static_cast<VideoCapture *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setAsync(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setAutoSave(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setOriginalFormat(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setSaveFormat(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setQuality(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setCaptureName(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setCaptureDir(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::VideoCapture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__VideoCapture.data,
      qt_meta_data_QtAV__VideoCapture,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::VideoCapture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::VideoCapture::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__VideoCapture.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::VideoCapture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::VideoCapture::requested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtAV::VideoCapture::frameAvailable(const QtAV::VideoFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtAV::VideoCapture::imageCaptured(const QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtAV::VideoCapture::failed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::VideoCapture::saved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QtAV::VideoCapture::asyncChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void QtAV::VideoCapture::autoSaveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void QtAV::VideoCapture::originalFormatChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void QtAV::VideoCapture::saveFormatChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void QtAV::VideoCapture::qualityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void QtAV::VideoCapture::captureNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void QtAV::VideoCapture::captureDirChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
