/****************************************************************************
** Meta object code from reading C++ file 'mainclass.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainclass.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainclass.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainClass_t {
    QByteArrayData data[9];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainClass_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainClass_t qt_meta_stringdata_MainClass = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MainClass"
QT_MOC_LITERAL(1, 10, 9), // "rvEEWInfo"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "_EEWInfo"
QT_MOC_LITERAL(4, 30, 12), // "doRepeatWork"
QT_MOC_LITERAL(5, 43, 11), // "extractQSCD"
QT_MOC_LITERAL(6, 55, 33), // "QMultiMap<int,_QSCD_FOR_MULTI..."
QT_MOC_LITERAL(7, 89, 17), // "recvUpdateMessage"
QT_MOC_LITERAL(8, 107, 15) // "_UPDATE_MESSAGE"

    },
    "MainClass\0rvEEWInfo\0\0_EEWInfo\0"
    "doRepeatWork\0extractQSCD\0"
    "QMultiMap<int,_QSCD_FOR_MULTIMAP>\0"
    "recvUpdateMessage\0_UPDATE_MESSAGE"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainClass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    0,   37,    2, 0x08 /* Private */,
       5,    1,   38,    2, 0x08 /* Private */,
       7,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,

       0        // eod
};

void MainClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainClass *_t = static_cast<MainClass *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rvEEWInfo((*reinterpret_cast< _EEWInfo(*)>(_a[1]))); break;
        case 1: _t->doRepeatWork(); break;
        case 2: _t->extractQSCD((*reinterpret_cast< QMultiMap<int,_QSCD_FOR_MULTIMAP>(*)>(_a[1]))); break;
        case 3: _t->recvUpdateMessage((*reinterpret_cast< _UPDATE_MESSAGE(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainClass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainClass.data,
      qt_meta_data_MainClass,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainClass::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainClass.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MainClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
