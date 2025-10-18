/****************************************************************************
** Meta object code from reading C++ file 'simplemode.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../simplemode.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simplemode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10SimpleModeE_t {};
} // unnamed namespace

template <> constexpr inline auto SimpleMode::qt_create_metaobjectdata<qt_meta_tag_ZN10SimpleModeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SimpleMode",
        "win",
        "",
        "timeOut",
        "exitToMenu",
        "on_exitBtn_clicked",
        "pauseGame",
        "resumeGame",
        "onSaveBtnClicked",
        "onLoadBtnClicked",
        "onContinueBtnClicked",
        "onExitBtnClicked",
        "getSaveData",
        "SaveData",
        "applySaveData",
        "data"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'win'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'timeOut'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'exitToMenu'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_exitBtn_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pauseGame'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'resumeGame'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSaveBtnClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadBtnClicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onContinueBtnClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onExitBtnClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'getSaveData'
        QtMocHelpers::SlotData<SaveData() const>(12, 2, QMC::AccessPrivate, 0x80000000 | 13),
        // Slot 'applySaveData'
        QtMocHelpers::SlotData<void(const SaveData &)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 13, 15 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SimpleMode, qt_meta_tag_ZN10SimpleModeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SimpleMode::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleModeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleModeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10SimpleModeE_t>.metaTypes,
    nullptr
} };

void SimpleMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SimpleMode *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->win(); break;
        case 1: _t->timeOut(); break;
        case 2: _t->exitToMenu(); break;
        case 3: _t->on_exitBtn_clicked(); break;
        case 4: _t->pauseGame(); break;
        case 5: _t->resumeGame(); break;
        case 6: _t->onSaveBtnClicked(); break;
        case 7: _t->onLoadBtnClicked(); break;
        case 8: _t->onContinueBtnClicked(); break;
        case 9: _t->onExitBtnClicked(); break;
        case 10: { SaveData _r = _t->getSaveData();
            if (_a[0]) *reinterpret_cast< SaveData*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->applySaveData((*reinterpret_cast< std::add_pointer_t<SaveData>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SimpleMode::*)()>(_a, &SimpleMode::win, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimpleMode::*)()>(_a, &SimpleMode::timeOut, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimpleMode::*)()>(_a, &SimpleMode::exitToMenu, 2))
            return;
    }
}

const QMetaObject *SimpleMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleMode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10SimpleModeE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int SimpleMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SimpleMode::win()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SimpleMode::timeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SimpleMode::exitToMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
