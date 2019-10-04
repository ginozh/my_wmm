/****************************************************************************
** Meta object code from reading C++ file 'FrameReader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/FrameReader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrameReader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__FrameReader_t {
    QByteArrayData data[13];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__FrameReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__FrameReader_t qt_meta_stringdata_QtAV__FrameReader = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QtAV::FrameReader"
QT_MOC_LITERAL(1, 18, 9), // "frameRead"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "QtAV::VideoFrame"
QT_MOC_LITERAL(4, 46, 5), // "frame"
QT_MOC_LITERAL(5, 52, 7), // "readEnd"
QT_MOC_LITERAL(6, 60, 12), // "seekFinished"
QT_MOC_LITERAL(7, 73, 3), // "pos"
QT_MOC_LITERAL(8, 77, 17), // "readMoreRequested"
QT_MOC_LITERAL(9, 95, 13), // "seekRequested"
QT_MOC_LITERAL(10, 109, 16), // "readMoreInternal"
QT_MOC_LITERAL(11, 126, 12), // "seekInternal"
QT_MOC_LITERAL(12, 139, 5) // "value"

    },
    "QtAV::FrameReader\0frameRead\0\0"
    "QtAV::VideoFrame\0frame\0readEnd\0"
    "seekFinished\0pos\0readMoreRequested\0"
    "seekRequested\0readMoreInternal\0"
    "seekInternal\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__FrameReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,
       6,    1,   53,    2, 0x06 /* Public */,
       8,    0,   56,    2, 0x06 /* Public */,
       9,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   60,    2, 0x08 /* Private */,
      11,    1,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::LongLong,   12,

       0        // eod
};

void QtAV::FrameReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FrameReader *_t = static_cast<FrameReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameRead((*reinterpret_cast< const QtAV::VideoFrame(*)>(_a[1]))); break;
        case 1: _t->readEnd(); break;
        case 2: _t->seekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 3: _t->readMoreRequested(); break;
        case 4: _t->seekRequested((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 5: _t->readMoreInternal(); break;
        case 6: { bool _r = _t->seekInternal((*reinterpret_cast< qint64(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
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
            typedef void (FrameReader::*_t)(const QtAV::VideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameReader::frameRead)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FrameReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameReader::readEnd)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FrameReader::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameReader::seekFinished)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (FrameReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameReader::readMoreRequested)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (FrameReader::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FrameReader::seekRequested)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QtAV::FrameReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__FrameReader.data,
      qt_meta_data_QtAV__FrameReader,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::FrameReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::FrameReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__FrameReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::FrameReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QtAV::FrameReader::frameRead(const QtAV::VideoFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::FrameReader::readEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::FrameReader::seekFinished(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtAV::FrameReader::readMoreRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::FrameReader::seekRequested(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
