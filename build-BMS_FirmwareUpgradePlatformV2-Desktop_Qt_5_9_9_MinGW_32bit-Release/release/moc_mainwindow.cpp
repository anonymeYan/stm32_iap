/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Qt_BMS_485IAP_V2/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[312];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 19), // "on_btn_file_clicked"
QT_MOC_LITERAL(4, 54, 21), // "on_btn_cleaar_clicked"
QT_MOC_LITERAL(5, 76, 19), // "on_btn_menu_clicked"
QT_MOC_LITERAL(6, 96, 20), // "on_btn_erase_clicked"
QT_MOC_LITERAL(7, 117, 21), // "on_btn_update_clicked"
QT_MOC_LITERAL(8, 139, 18), // "on_btn_run_clicked"
QT_MOC_LITERAL(9, 158, 20), // "on_btn_write_clicked"
QT_MOC_LITERAL(10, 179, 18), // "SerialRecvMsgEvent"
QT_MOC_LITERAL(11, 198, 16), // "transmitProgress"
QT_MOC_LITERAL(12, 215, 8), // "progress"
QT_MOC_LITERAL(13, 224, 14), // "transmitStatus"
QT_MOC_LITERAL(14, 239, 18), // "YmodemSend::Status"
QT_MOC_LITERAL(15, 258, 6), // "status"
QT_MOC_LITERAL(16, 265, 19), // "transmitDebugMessge"
QT_MOC_LITERAL(17, 285, 6), // "Messge"
QT_MOC_LITERAL(18, 292, 19) // "on_btn_copy_clicked"

    },
    "MainWindow\0on_pushButton_clicked\0\0"
    "on_btn_file_clicked\0on_btn_cleaar_clicked\0"
    "on_btn_menu_clicked\0on_btn_erase_clicked\0"
    "on_btn_update_clicked\0on_btn_run_clicked\0"
    "on_btn_write_clicked\0SerialRecvMsgEvent\0"
    "transmitProgress\0progress\0transmitStatus\0"
    "YmodemSend::Status\0status\0transmitDebugMessge\0"
    "Messge\0on_btn_copy_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    1,   88,    2, 0x08 /* Private */,
      13,    1,   91,    2, 0x08 /* Private */,
      16,    1,   94,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_btn_file_clicked(); break;
        case 2: _t->on_btn_cleaar_clicked(); break;
        case 3: _t->on_btn_menu_clicked(); break;
        case 4: _t->on_btn_erase_clicked(); break;
        case 5: _t->on_btn_update_clicked(); break;
        case 6: _t->on_btn_run_clicked(); break;
        case 7: _t->on_btn_write_clicked(); break;
        case 8: _t->SerialRecvMsgEvent(); break;
        case 9: _t->transmitProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->transmitStatus((*reinterpret_cast< YmodemSend::Status(*)>(_a[1]))); break;
        case 11: _t->transmitDebugMessge((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->on_btn_copy_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
