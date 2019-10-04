/****************************************************************************
** Meta object code from reading C++ file 'PropertyEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../examples/player/config/PropertyEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PropertyEditor_t {
    QByteArrayData data[11];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertyEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertyEditor_t qt_meta_stringdata_PropertyEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PropertyEditor"
QT_MOC_LITERAL(1, 15, 12), // "onFlagChange"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 8), // "QAction*"
QT_MOC_LITERAL(4, 38, 6), // "action"
QT_MOC_LITERAL(5, 45, 12), // "onEnumChange"
QT_MOC_LITERAL(6, 58, 5), // "value"
QT_MOC_LITERAL(7, 64, 11), // "onIntChange"
QT_MOC_LITERAL(8, 76, 12), // "onRealChange"
QT_MOC_LITERAL(9, 89, 12), // "onTextChange"
QT_MOC_LITERAL(10, 102, 12) // "onBoolChange"

    },
    "PropertyEditor\0onFlagChange\0\0QAction*\0"
    "action\0onEnumChange\0value\0onIntChange\0"
    "onRealChange\0onTextChange\0onBoolChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertyEditor[] = {

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
       1,    1,   44,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,
       9,    1,   56,    2, 0x08 /* Private */,
      10,    1,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QReal,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void PropertyEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertyEditor *_t = static_cast<PropertyEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onFlagChange((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->onEnumChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onIntChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onRealChange((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 4: _t->onTextChange((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onBoolChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PropertyEditor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PropertyEditor.data,
      qt_meta_data_PropertyEditor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PropertyEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PropertyEditor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
