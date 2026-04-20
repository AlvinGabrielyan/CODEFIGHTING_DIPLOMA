/****************************************************************************
** Meta object code from reading C++ file 'bee.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../bee.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bee.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN3BeeE_t {};
} // unnamed namespace

template <> constexpr inline auto Bee::qt_create_metaobjectdata<qt_meta_tag_ZN3BeeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Bee",
        "locationChanged",
        "",
        "targetLocationChanged",
        "energyChanged",
        "currentNectarChanged",
        "nectarTypeChanged",
        "isFlyingChanged",
        "progressChanged",
        "positionXChanged",
        "positionYChanged",
        "canFly",
        "calculateFlightTime",
        "consumeEnergy",
        "amount",
        "collectNectar",
        "type",
        "depositNectar",
        "isEmpty",
        "isFull",
        "id",
        "name",
        "location",
        "targetLocation",
        "energy",
        "capacity",
        "currentNectar",
        "nectarType",
        "isFlying",
        "progress",
        "positionX",
        "positionY",
        "color",
        "QColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'locationChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'targetLocationChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'energyChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentNectarChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'nectarTypeChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isFlyingChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'progressChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionXChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionYChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'canFly'
        QtMocHelpers::MethodData<bool() const>(11, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'calculateFlightTime'
        QtMocHelpers::MethodData<int() const>(12, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'consumeEnergy'
        QtMocHelpers::MethodData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Method 'collectNectar'
        QtMocHelpers::MethodData<bool(const QString &, double)>(15, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 16 }, { QMetaType::Double, 14 },
        }}),
        // Method 'depositNectar'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'isEmpty'
        QtMocHelpers::MethodData<bool() const>(18, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'isFull'
        QtMocHelpers::MethodData<bool() const>(19, 2, QMC::AccessPublic, QMetaType::Bool),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'id'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'name'
        QtMocHelpers::PropertyData<QString>(21, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'location'
        QtMocHelpers::PropertyData<QString>(22, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'targetLocation'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'energy'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'capacity'
        QtMocHelpers::PropertyData<double>(25, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'currentNectar'
        QtMocHelpers::PropertyData<double>(26, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'nectarType'
        QtMocHelpers::PropertyData<QString>(27, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'isFlying'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'progress'
        QtMocHelpers::PropertyData<double>(29, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'positionX'
        QtMocHelpers::PropertyData<double>(30, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'positionY'
        QtMocHelpers::PropertyData<double>(31, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 8),
        // property 'color'
        QtMocHelpers::PropertyData<QColor>(32, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Bee, qt_meta_tag_ZN3BeeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Bee::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3BeeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3BeeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN3BeeE_t>.metaTypes,
    nullptr
} };

void Bee::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Bee *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->locationChanged(); break;
        case 1: _t->targetLocationChanged(); break;
        case 2: _t->energyChanged(); break;
        case 3: _t->currentNectarChanged(); break;
        case 4: _t->nectarTypeChanged(); break;
        case 5: _t->isFlyingChanged(); break;
        case 6: _t->progressChanged(); break;
        case 7: _t->positionXChanged(); break;
        case 8: _t->positionYChanged(); break;
        case 9: { bool _r = _t->canFly();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->calculateFlightTime();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->consumeEnergy((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: { bool _r = _t->collectNectar((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->depositNectar(); break;
        case 14: { bool _r = _t->isEmpty();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->isFull();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::locationChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::targetLocationChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::energyChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::currentNectarChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::nectarTypeChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::isFlyingChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::progressChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::positionXChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Bee::*)()>(_a, &Bee::positionYChanged, 8))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->id(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->location(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->targetLocation(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->energy(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->capacity(); break;
        case 6: *reinterpret_cast<double*>(_v) = _t->currentNectar(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->nectarType(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->isFlying(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->progress(); break;
        case 10: *reinterpret_cast<double*>(_v) = _t->positionX(); break;
        case 11: *reinterpret_cast<double*>(_v) = _t->positionY(); break;
        case 12: *reinterpret_cast<QColor*>(_v) = _t->color(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setLocation(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setTargetLocation(*reinterpret_cast<QString*>(_v)); break;
        case 4: _t->setEnergy(*reinterpret_cast<int*>(_v)); break;
        case 6: _t->setCurrentNectar(*reinterpret_cast<double*>(_v)); break;
        case 7: _t->setNectarType(*reinterpret_cast<QString*>(_v)); break;
        case 8: _t->setIsFlying(*reinterpret_cast<bool*>(_v)); break;
        case 9: _t->setProgress(*reinterpret_cast<double*>(_v)); break;
        case 10: _t->setPositionX(*reinterpret_cast<double*>(_v)); break;
        case 11: _t->setPositionY(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Bee::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Bee::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3BeeE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Bee::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Bee::locationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Bee::targetLocationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Bee::energyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Bee::currentNectarChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Bee::nectarTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Bee::isFlyingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Bee::progressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Bee::positionXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Bee::positionYChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
