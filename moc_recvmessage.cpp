/****************************************************************************
** Meta object code from reading C++ file 'recvmessage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "recvmessage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recvmessage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RecvMessage_t {
    QByteArrayData data[4];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecvMessage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecvMessage_t qt_meta_stringdata_RecvMessage = {
    {
QT_MOC_LITERAL(0, 0, 11), // "RecvMessage"
QT_MOC_LITERAL(1, 12, 14), // "sendQSCDtoMain"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 33) // "QMultiMap<int,_QSCD_FOR_MULTI..."

    },
    "RecvMessage\0sendQSCDtoMain\0\0"
    "QMultiMap<int,_QSCD_FOR_MULTIMAP>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecvMessage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void RecvMessage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecvMessage *_t = static_cast<RecvMessage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendQSCDtoMain((*reinterpret_cast< QMultiMap<int,_QSCD_FOR_MULTIMAP>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RecvMessage::*_t)(QMultiMap<int,_QSCD_FOR_MULTIMAP> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RecvMessage::sendQSCDtoMain)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RecvMessage::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RecvMessage.data,
      qt_meta_data_RecvMessage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RecvMessage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecvMessage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecvMessage.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int RecvMessage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void RecvMessage::sendQSCDtoMain(QMultiMap<int,_QSCD_FOR_MULTIMAP> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_RecvEEWMessage_t {
    QByteArrayData data[4];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecvEEWMessage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecvEEWMessage_t qt_meta_stringdata_RecvEEWMessage = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RecvEEWMessage"
QT_MOC_LITERAL(1, 15, 10), // "_rvEEWInfo"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8) // "_EEWInfo"

    },
    "RecvEEWMessage\0_rvEEWInfo\0\0_EEWInfo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecvEEWMessage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void RecvEEWMessage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecvEEWMessage *_t = static_cast<RecvEEWMessage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->_rvEEWInfo((*reinterpret_cast< _EEWInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RecvEEWMessage::*_t)(_EEWInfo );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RecvEEWMessage::_rvEEWInfo)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RecvEEWMessage::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RecvEEWMessage.data,
      qt_meta_data_RecvEEWMessage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RecvEEWMessage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecvEEWMessage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecvEEWMessage.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int RecvEEWMessage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void RecvEEWMessage::_rvEEWInfo(_EEWInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_RecvUpdateMessage_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RecvUpdateMessage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RecvUpdateMessage_t qt_meta_stringdata_RecvUpdateMessage = {
    {
QT_MOC_LITERAL(0, 0, 17), // "RecvUpdateMessage"
QT_MOC_LITERAL(1, 18, 13), // "_rvUpdateInfo"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 15) // "_UPDATE_MESSAGE"

    },
    "RecvUpdateMessage\0_rvUpdateInfo\0\0"
    "_UPDATE_MESSAGE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RecvUpdateMessage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void RecvUpdateMessage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RecvUpdateMessage *_t = static_cast<RecvUpdateMessage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->_rvUpdateInfo((*reinterpret_cast< _UPDATE_MESSAGE(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RecvUpdateMessage::*_t)(_UPDATE_MESSAGE );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RecvUpdateMessage::_rvUpdateInfo)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RecvUpdateMessage::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RecvUpdateMessage.data,
      qt_meta_data_RecvUpdateMessage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RecvUpdateMessage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RecvUpdateMessage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RecvUpdateMessage.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int RecvUpdateMessage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void RecvUpdateMessage::_rvUpdateInfo(_UPDATE_MESSAGE _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
