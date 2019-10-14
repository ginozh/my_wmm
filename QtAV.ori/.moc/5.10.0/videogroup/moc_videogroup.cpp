/****************************************************************************
** Meta object code from reading C++ file 'videogroup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../examples/videogroup/videogroup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videogroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoGroup_t {
    QByteArrayData data[10];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoGroup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoGroup_t qt_meta_stringdata_VideoGroup = {
    {
QT_MOC_LITERAL(0, 0, 10), // "VideoGroup"
QT_MOC_LITERAL(1, 11, 15), // "setSingleWindow"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "s"
QT_MOC_LITERAL(4, 30, 17), // "toggleSingleFrame"
QT_MOC_LITERAL(5, 48, 15), // "toggleFrameless"
QT_MOC_LITERAL(6, 64, 1), // "f"
QT_MOC_LITERAL(7, 66, 13), // "openLocalFile"
QT_MOC_LITERAL(8, 80, 11), // "addRenderer"
QT_MOC_LITERAL(9, 92, 14) // "removeRenderer"

    },
    "VideoGroup\0setSingleWindow\0\0s\0"
    "toggleSingleFrame\0toggleFrameless\0f\0"
    "openLocalFile\0addRenderer\0removeRenderer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoGroup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       5,    1,   50,    2, 0x0a /* Public */,
       7,    0,   53,    2, 0x0a /* Public */,
       8,    0,   54,    2, 0x0a /* Public */,
       9,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VideoGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoGroup *_t = static_cast<VideoGroup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setSingleWindow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->toggleSingleFrame((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->toggleFrameless((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->openLocalFile(); break;
        case 4: _t->addRenderer(); break;
        case 5: _t->removeRenderer(); break;
        default: ;
        }
    }
}

const QMetaObject VideoGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VideoGroup.data,
      qt_meta_data_VideoGroup,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoGroup.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
