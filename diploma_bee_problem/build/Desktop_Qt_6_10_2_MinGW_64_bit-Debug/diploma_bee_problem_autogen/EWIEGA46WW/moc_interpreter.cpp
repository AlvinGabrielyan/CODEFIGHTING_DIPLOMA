/****************************************************************************
** Meta object code from reading C++ file 'interpreter.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../interpreter.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interpreter.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11InterpreterE_t {};
} // unnamed namespace

template <> constexpr inline auto Interpreter::qt_create_metaobjectdata<qt_meta_tag_ZN11InterpreterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Interpreter",
        "errorsChanged",
        "",
        "warningsChanged",
        "parsingProgress",
        "percent",
        "parseScript",
        "QList<Command*>",
        "script",
        "validateCommand",
        "Command*",
        "cmd",
        "availableBees",
        "validateAllCommands",
        "commands",
        "estimateTotalTime",
        "suggestOptimizations",
        "errors",
        "warnings"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'errorsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'warningsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'parsingProgress'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Method 'parseScript'
        QtMocHelpers::MethodData<QList<Command*>(const QString &)>(6, 2, QMC::AccessPublic, 0x80000000 | 7, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'validateCommand'
        QtMocHelpers::MethodData<bool(Command *, const QStringList &)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 10, 11 }, { QMetaType::QStringList, 12 },
        }}),
        // Method 'validateAllCommands'
        QtMocHelpers::MethodData<bool(const QList<Command*> &)>(13, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { 0x80000000 | 7, 14 },
        }}),
        // Method 'estimateTotalTime'
        QtMocHelpers::MethodData<double(const QList<Command*> &)>(15, 2, QMC::AccessPublic, QMetaType::Double, {{
            { 0x80000000 | 7, 14 },
        }}),
        // Method 'suggestOptimizations'
        QtMocHelpers::MethodData<QStringList(const QList<Command*> &)>(16, 2, QMC::AccessPublic, QMetaType::QStringList, {{
            { 0x80000000 | 7, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'errors'
        QtMocHelpers::PropertyData<QStringList>(17, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'warnings'
        QtMocHelpers::PropertyData<QStringList>(18, QMetaType::QStringList, QMC::DefaultPropertyFlags, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Interpreter, qt_meta_tag_ZN11InterpreterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Interpreter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11InterpreterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11InterpreterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11InterpreterE_t>.metaTypes,
    nullptr
} };

void Interpreter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Interpreter *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->errorsChanged(); break;
        case 1: _t->warningsChanged(); break;
        case 2: _t->parsingProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: { QList<Command*> _r = _t->parseScript((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QList<Command*>*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->validateCommand((*reinterpret_cast<std::add_pointer_t<Command*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->validateAllCommands((*reinterpret_cast<std::add_pointer_t<QList<Command*>>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { double _r = _t->estimateTotalTime((*reinterpret_cast<std::add_pointer_t<QList<Command*>>>(_a[1])));
            if (_a[0]) *reinterpret_cast<double*>(_a[0]) = std::move(_r); }  break;
        case 7: { QStringList _r = _t->suggestOptimizations((*reinterpret_cast<std::add_pointer_t<QList<Command*>>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Command* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Command*> >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Command*> >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<Command*> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Interpreter::*)()>(_a, &Interpreter::errorsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Interpreter::*)()>(_a, &Interpreter::warningsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Interpreter::*)(int )>(_a, &Interpreter::parsingProgress, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QStringList*>(_v) = _t->errors(); break;
        case 1: *reinterpret_cast<QStringList*>(_v) = _t->warnings(); break;
        default: break;
        }
    }
}

const QMetaObject *Interpreter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Interpreter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11InterpreterE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Interpreter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Interpreter::errorsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Interpreter::warningsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Interpreter::parsingProgress(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
