/****************************************************************************
** Meta object code from reading C++ file 'boat.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../boat.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'boat.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN4BoatE_t {};
} // unnamed namespace

template <> constexpr inline auto Boat::qt_create_metaobjectdata<qt_meta_tag_ZN4BoatE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Boat",
        "sideChanged",
        "",
        "targetSideChanged",
        "fuelChanged",
        "cargoTypeChanged",
        "cargoWeightChanged",
        "isMovingChanged",
        "progressChanged",
        "positionXChanged",
        "canMove",
        "calculateCrossingTime",
        "consumeFuel",
        "amount",
        "loadCargo",
        "type",
        "weight",
        "unloadCargo",
        "canCarry",
        "cargoType",
        "id",
        "name",
        "side",
        "targetSide",
        "minSpeed",
        "maxSpeed",
        "capacity",
        "fuel",
        "color",
        "QColor",
        "description",
        "cargoWeight",
        "isMoving",
        "progress",
        "positionX"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sideChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'targetSideChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fuelChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cargoTypeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cargoWeightChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isMovingChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'progressChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionXChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'canMove'
        QtMocHelpers::MethodData<bool() const>(10, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'calculateCrossingTime'
        QtMocHelpers::MethodData<int() const>(11, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'consumeFuel'
        QtMocHelpers::MethodData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Method 'loadCargo'
        QtMocHelpers::MethodData<bool(const QString &, int)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 15 }, { QMetaType::Int, 16 },
        }}),
        // Method 'unloadCargo'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'canCarry'
        QtMocHelpers::MethodData<bool(const QString &) const>(18, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'id'
        QtMocHelpers::PropertyData<QString>(20, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'name'
        QtMocHelpers::PropertyData<QString>(21, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'side'
        QtMocHelpers::PropertyData<QString>(22, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'targetSide'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'minSpeed'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'maxSpeed'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'capacity'
        QtMocHelpers::PropertyData<int>(26, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'fuel'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'color'
        QtMocHelpers::PropertyData<QColor>(28, 0x80000000 | 29, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'description'
        QtMocHelpers::PropertyData<QString>(30, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'cargoType'
        QtMocHelpers::PropertyData<QString>(19, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'cargoWeight'
        QtMocHelpers::PropertyData<int>(31, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'isMoving'
        QtMocHelpers::PropertyData<bool>(32, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'progress'
        QtMocHelpers::PropertyData<double>(33, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'positionX'
        QtMocHelpers::PropertyData<double>(34, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Boat, qt_meta_tag_ZN4BoatE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Boat::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4BoatE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4BoatE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN4BoatE_t>.metaTypes,
    nullptr
} };

void Boat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Boat *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sideChanged(); break;
        case 1: _t->targetSideChanged(); break;
        case 2: _t->fuelChanged(); break;
        case 3: _t->cargoTypeChanged(); break;
        case 4: _t->cargoWeightChanged(); break;
        case 5: _t->isMovingChanged(); break;
        case 6: _t->progressChanged(); break;
        case 7: _t->positionXChanged(); break;
        case 8: { bool _r = _t->canMove();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->calculateCrossingTime();
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->consumeFuel((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 11: { bool _r = _t->loadCargo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->unloadCargo(); break;
        case 13: { bool _r = _t->canCarry((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::sideChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::targetSideChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::fuelChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::cargoTypeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::cargoWeightChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::isMovingChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::progressChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Boat::*)()>(_a, &Boat::positionXChanged, 7))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->id(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->side(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->targetSide(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->minSpeed(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->maxSpeed(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->capacity(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->fuel(); break;
        case 8: *reinterpret_cast<QColor*>(_v) = _t->color(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->description(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->cargoType(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->cargoWeight(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->isMoving(); break;
        case 13: *reinterpret_cast<double*>(_v) = _t->progress(); break;
        case 14: *reinterpret_cast<double*>(_v) = _t->positionX(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setSide(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setTargetSide(*reinterpret_cast<QString*>(_v)); break;
        case 7: _t->setFuel(*reinterpret_cast<int*>(_v)); break;
        case 10: _t->setCargoType(*reinterpret_cast<QString*>(_v)); break;
        case 11: _t->setCargoWeight(*reinterpret_cast<int*>(_v)); break;
        case 12: _t->setIsMoving(*reinterpret_cast<bool*>(_v)); break;
        case 13: _t->setProgress(*reinterpret_cast<double*>(_v)); break;
        case 14: _t->setPositionX(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Boat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Boat::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN4BoatE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Boat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Boat::sideChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Boat::targetSideChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Boat::fuelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Boat::cargoTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Boat::cargoWeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Boat::isMovingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Boat::progressChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Boat::positionXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
