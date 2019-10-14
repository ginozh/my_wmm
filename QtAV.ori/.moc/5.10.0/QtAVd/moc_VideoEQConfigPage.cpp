/****************************************************************************
** Meta object code from reading C++ file 'VideoEQConfigPage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../examples/player/config/VideoEQConfigPage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoEQConfigPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoEQConfigPage_t {
    QByteArrayData data[10];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoEQConfigPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoEQConfigPage_t qt_meta_stringdata_VideoEQConfigPage = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VideoEQConfigPage"
QT_MOC_LITERAL(1, 18, 13), // "engineChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 17), // "brightnessChanged"
QT_MOC_LITERAL(4, 51, 15), // "contrastChanged"
QT_MOC_LITERAL(5, 67, 10), // "hueChanegd"
QT_MOC_LITERAL(6, 78, 17), // "saturationChanged"
QT_MOC_LITERAL(7, 96, 11), // "onGlobalSet"
QT_MOC_LITERAL(8, 108, 7), // "onReset"
QT_MOC_LITERAL(9, 116, 19) // "onEngineChangedByUI"

    },
    "VideoEQConfigPage\0engineChanged\0\0"
    "brightnessChanged\0contrastChanged\0"
    "hueChanegd\0saturationChanged\0onGlobalSet\0"
    "onReset\0onEngineChangedByUI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoEQConfigPage[] = {

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
       3,    1,   55,    2, 0x06 /* Public */,
       4,    1,   58,    2, 0x06 /* Public */,
       5,    1,   61,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   67,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VideoEQConfigPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoEQConfigPage *_t = static_cast<VideoEQConfigPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->engineChanged(); break;
        case 1: _t->brightnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->contrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->hueChanegd((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->saturationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onGlobalSet((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onReset(); break;
        case 7: _t->onEngineChangedByUI(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoEQConfigPage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEQConfigPage::engineChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoEQConfigPage::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEQConfigPage::brightnessChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoEQConfigPage::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEQConfigPage::contrastChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoEQConfigPage::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEQConfigPage::hueChanegd)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoEQConfigPage::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoEQConfigPage::saturationChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject VideoEQConfigPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VideoEQConfigPage.data,
      qt_meta_data_VideoEQConfigPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoEQConfigPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoEQConfigPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoEQConfigPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VideoEQConfigPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void VideoEQConfigPage::engineChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoEQConfigPage::brightnessChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoEQConfigPage::contrastChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoEQConfigPage::hueChanegd(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoEQConfigPage::saturationChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
