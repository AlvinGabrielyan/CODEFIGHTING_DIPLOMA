/****************************************************************************
** Meta object code from reading C++ file 'command.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../command.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
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
struct qt_meta_tag_ZN7CommandE_t {};
} // unnamed namespace

template <> constexpr inline auto Command::qt_create_metaobjectdata<qt_meta_tag_ZN7CommandE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Command",
        "executedChanged",
        "",
        "toString",
        "typeToString",
        "CommandType",
        "type",
        "boatId",
        "params",
        "QVariantMap",
        "executeTime",
        "lineNumber",
        "executed",
        "SEND",
        "REFUEL",
        "LOAD",
        "UNLOAD",
        "CHECK_BRIDGE",
        "REPAIR_BRIDGE",
        "UNKNOWN"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'executedChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toString'
        QtMocHelpers::MethodData<QString() const>(3, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'typeToString'
        QtMocHelpers::MethodData<QString(enum CommandType)>(4, 2, QMC::AccessPublic, QMetaType::QString, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'type'
        QtMocHelpers::PropertyData<enum CommandType>(6, 0x80000000 | 5, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'boatId'
        QtMocHelpers::PropertyData<QString>(7, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'params'
        QtMocHelpers::PropertyData<QVariantMap>(8, 0x80000000 | 9, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'executeTime'
        QtMocHelpers::PropertyData<double>(10, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'lineNumber'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'executed'
        QtMocHelpers::PropertyData<bool>(12, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'CommandType'
        QtMocHelpers::EnumData<enum CommandType>(5, 5, QMC::EnumFlags{}).add({
            {   13, CommandType::SEND },
            {   14, CommandType::REFUEL },
            {   15, CommandType::LOAD },
            {   16, CommandType::UNLOAD },
            {   17, CommandType::CHECK_BRIDGE },
            {   18, CommandType::REPAIR_BRIDGE },
            {   19, CommandType::UNKNOWN },
        }),
    };
    return QtMocHelpers::metaObjectData<Command, qt_meta_tag_ZN7CommandE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Command::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7CommandE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7CommandE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7CommandE_t>.metaTypes,
    nullptr
} };

void Command::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Command *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->executedChanged(); break;
        case 1: { QString _r = _t->toString();
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        case 2: { QString _r = _t->typeToString((*reinterpret_cast<std::add_pointer_t<enum CommandType>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Command::*)()>(_a, &Command::executedChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<enum CommandType*>(_v) = _t->type(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->boatId(); break;
        case 2: *reinterpret_cast<QVariantMap*>(_v) = _t->params(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->executeTime(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->lineNumber(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->executed(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 5: _t->setExecuted(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Command::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Command::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7CommandE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Command::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Command::executedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
