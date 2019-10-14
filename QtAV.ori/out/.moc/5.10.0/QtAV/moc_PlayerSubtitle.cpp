/****************************************************************************
** Meta object code from reading C++ file 'PlayerSubtitle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/private/PlayerSubtitle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlayerSubtitle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__PlayerSubtitle_t {
    QByteArrayData data[20];
    char stringdata0[297];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__PlayerSubtitle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__PlayerSubtitle_t qt_meta_stringdata_QtAV__PlayerSubtitle = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QtAV::PlayerSubtitle"
QT_MOC_LITERAL(1, 21, 15), // "autoLoadChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "value"
QT_MOC_LITERAL(4, 44, 11), // "fileChanged"
QT_MOC_LITERAL(5, 56, 16), // "onEnabledChanged"
QT_MOC_LITERAL(6, 73, 21), // "onPlayerSourceChanged"
QT_MOC_LITERAL(7, 95, 23), // "onPlayerPositionChanged"
QT_MOC_LITERAL(8, 119, 13), // "onPlayerStart"
QT_MOC_LITERAL(9, 133, 9), // "tryReload"
QT_MOC_LITERAL(10, 143, 20), // "tryReloadInternalSub"
QT_MOC_LITERAL(11, 164, 28), // "updateInternalSubtitleTracks"
QT_MOC_LITERAL(12, 193, 6), // "tracks"
QT_MOC_LITERAL(13, 200, 29), // "processInternalSubtitlePacket"
QT_MOC_LITERAL(14, 230, 5), // "track"
QT_MOC_LITERAL(15, 236, 12), // "QtAV::Packet"
QT_MOC_LITERAL(16, 249, 6), // "packet"
QT_MOC_LITERAL(17, 256, 29), // "processInternalSubtitleHeader"
QT_MOC_LITERAL(18, 286, 5), // "codec"
QT_MOC_LITERAL(19, 292, 4) // "data"

    },
    "QtAV::PlayerSubtitle\0autoLoadChanged\0"
    "\0value\0fileChanged\0onEnabledChanged\0"
    "onPlayerSourceChanged\0onPlayerPositionChanged\0"
    "onPlayerStart\0tryReload\0tryReloadInternalSub\0"
    "updateInternalSubtitleTracks\0tracks\0"
    "processInternalSubtitlePacket\0track\0"
    "QtAV::Packet\0packet\0processInternalSubtitleHeader\0"
    "codec\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__PlayerSubtitle[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   73,    2, 0x0a /* Public */,
       6,    0,   76,    2, 0x08 /* Private */,
       7,    0,   77,    2, 0x08 /* Private */,
       8,    0,   78,    2, 0x08 /* Private */,
       9,    0,   79,    2, 0x08 /* Private */,
      10,    0,   80,    2, 0x08 /* Private */,
      11,    1,   81,    2, 0x08 /* Private */,
      13,    2,   84,    2, 0x08 /* Private */,
      17,    2,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariantList,   12,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   14,   16,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::QByteArray,   18,   19,

       0        // eod
};

void QtAV::PlayerSubtitle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlayerSubtitle *_t = static_cast<PlayerSubtitle *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->autoLoadChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->fileChanged(); break;
        case 2: _t->onEnabledChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onPlayerSourceChanged(); break;
        case 4: _t->onPlayerPositionChanged(); break;
        case 5: _t->onPlayerStart(); break;
        case 6: _t->tryReload(); break;
        case 7: _t->tryReloadInternalSub(); break;
        case 8: _t->updateInternalSubtitleTracks((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 9: _t->processInternalSubtitlePacket((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QtAV::Packet(*)>(_a[2]))); break;
        case 10: _t->processInternalSubtitleHeader((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::Packet >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (PlayerSubtitle::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayerSubtitle::autoLoadChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (PlayerSubtitle::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayerSubtitle::fileChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QtAV::PlayerSubtitle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__PlayerSubtitle.data,
      qt_meta_data_QtAV__PlayerSubtitle,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::PlayerSubtitle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::PlayerSubtitle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__PlayerSubtitle.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::PlayerSubtitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QtAV::PlayerSubtitle::autoLoadChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::PlayerSubtitle::fileChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
