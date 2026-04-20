/****************************************************************************
** Meta object code from reading C++ file 'simulator.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../simulator.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulator.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9SimulatorE_t {};
} // unnamed namespace

template <> constexpr inline auto Simulator::qt_create_metaobjectdata<qt_meta_tag_ZN9SimulatorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Simulator",
        "currentTimeChanged",
        "",
        "isRunningChanged",
        "speedChanged",
        "bridgeHealthChanged",
        "bridgeOccupiedChanged",
        "completedBoatsChanged",
        "totalBoatsChanged",
        "collisionsChanged",
        "totalScoreChanged",
        "starsChanged",
        "logMessage",
        "message",
        "type",
        "commandExecuted",
        "Command*",
        "command",
        "success",
        "boatStartedCrossing",
        "Boat*",
        "boat",
        "boatFinishedCrossing",
        "collisionDetected",
        "boat1",
        "boat2",
        "simulationFinished",
        "update",
        "addBoat",
        "getBoat",
        "id",
        "getAllBoats",
        "QList<QObject*>",
        "loadScript",
        "script",
        "getErrors",
        "getWarnings",
        "start",
        "pause",
        "stop",
        "reset",
        "step",
        "currentTime",
        "isRunning",
        "speed",
        "bridgeHealth",
        "bridgeOccupied",
        "completedBoats",
        "totalBoats",
        "collisions",
        "totalScore",
        "stars"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentTimeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRunningChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'speedChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bridgeHealthChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bridgeOccupiedChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'completedBoatsChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'totalBoatsChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'collisionsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'totalScoreChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'starsChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
        }}),
        // Signal 'commandExecuted'
        QtMocHelpers::SignalData<void(Command *, bool)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { QMetaType::Bool, 18 },
        }}),
        // Signal 'boatStartedCrossing'
        QtMocHelpers::SignalData<void(Boat *)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Signal 'boatFinishedCrossing'
        QtMocHelpers::SignalData<void(Boat *)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Signal 'collisionDetected'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 },
        }}),
        // Signal 'simulationFinished'
        QtMocHelpers::SignalData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'update'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'addBoat'
        QtMocHelpers::MethodData<void(Boat *)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Method 'getBoat'
        QtMocHelpers::MethodData<Boat *(const QString &)>(29, 2, QMC::AccessPublic, 0x80000000 | 20, {{
            { QMetaType::QString, 30 },
        }}),
        // Method 'getAllBoats'
        QtMocHelpers::MethodData<QList<QObject*>()>(31, 2, QMC::AccessPublic, 0x80000000 | 32),
        // Method 'loadScript'
        QtMocHelpers::MethodData<bool(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 34 },
        }}),
        // Method 'getErrors'
        QtMocHelpers::MethodData<QStringList() const>(35, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'getWarnings'
        QtMocHelpers::MethodData<QStringList() const>(36, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'start'
        QtMocHelpers::MethodData<void()>(37, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause'
        QtMocHelpers::MethodData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stop'
        QtMocHelpers::MethodData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reset'
        QtMocHelpers::MethodData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'step'
        QtMocHelpers::MethodData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentTime'
        QtMocHelpers::PropertyData<double>(42, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'speed'
        QtMocHelpers::PropertyData<double>(44, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'bridgeHealth'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'bridgeOccupied'
        QtMocHelpers::PropertyData<bool>(46, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'completedBoats'
        QtMocHelpers::PropertyData<int>(47, QMetaType::Int, QMC::DefaultPropertyFlags, 5),
        // property 'totalBoats'
        QtMocHelpers::PropertyData<int>(48, QMetaType::Int, QMC::DefaultPropertyFlags, 6),
        // property 'collisions'
        QtMocHelpers::PropertyData<int>(49, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'totalScore'
        QtMocHelpers::PropertyData<int>(50, QMetaType::Int, QMC::DefaultPropertyFlags, 8),
        // property 'stars'
        QtMocHelpers::PropertyData<int>(51, QMetaType::Int, QMC::DefaultPropertyFlags, 9),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Simulator, qt_meta_tag_ZN9SimulatorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Simulator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9SimulatorE_t>.metaTypes,
    nullptr
} };

void Simulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Simulator *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentTimeChanged(); break;
        case 1: _t->isRunningChanged(); break;
        case 2: _t->speedChanged(); break;
        case 3: _t->bridgeHealthChanged(); break;
        case 4: _t->bridgeOccupiedChanged(); break;
        case 5: _t->completedBoatsChanged(); break;
        case 6: _t->totalBoatsChanged(); break;
        case 7: _t->collisionsChanged(); break;
        case 8: _t->totalScoreChanged(); break;
        case 9: _t->starsChanged(); break;
        case 10: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->commandExecuted((*reinterpret_cast<std::add_pointer_t<Command*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 12: _t->boatStartedCrossing((*reinterpret_cast<std::add_pointer_t<Boat*>>(_a[1]))); break;
        case 13: _t->boatFinishedCrossing((*reinterpret_cast<std::add_pointer_t<Boat*>>(_a[1]))); break;
        case 14: _t->collisionDetected((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 15: _t->simulationFinished(); break;
        case 16: _t->update(); break;
        case 17: _t->addBoat((*reinterpret_cast<std::add_pointer_t<Boat*>>(_a[1]))); break;
        case 18: { Boat* _r = _t->getBoat((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<Boat**>(_a[0]) = std::move(_r); }  break;
        case 19: { QList<QObject*> _r = _t->getAllBoats();
            if (_a[0]) *reinterpret_cast<QList<QObject*>*>(_a[0]) = std::move(_r); }  break;
        case 20: { bool _r = _t->loadScript((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 21: { QStringList _r = _t->getErrors();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 22: { QStringList _r = _t->getWarnings();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->start(); break;
        case 24: _t->pause(); break;
        case 25: _t->stop(); break;
        case 26: _t->reset(); break;
        case 27: _t->step(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Command* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Boat* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Boat* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Boat* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::currentTimeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::isRunningChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::speedChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::bridgeHealthChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::bridgeOccupiedChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::completedBoatsChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::totalBoatsChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::collisionsChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::totalScoreChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::starsChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(const QString & , const QString & )>(_a, &Simulator::logMessage, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(Command * , bool )>(_a, &Simulator::commandExecuted, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(Boat * )>(_a, &Simulator::boatStartedCrossing, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(Boat * )>(_a, &Simulator::boatFinishedCrossing, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)(const QString & , const QString & )>(_a, &Simulator::collisionDetected, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Simulator::*)()>(_a, &Simulator::simulationFinished, 15))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->currentTime(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->speed(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->bridgeHealth(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->bridgeOccupied(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->completedBoats(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->totalBoats(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->collisions(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->totalScore(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->stars(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setSpeed(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Simulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Simulator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9SimulatorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Simulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Simulator::currentTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Simulator::isRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Simulator::speedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Simulator::bridgeHealthChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Simulator::bridgeOccupiedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Simulator::completedBoatsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void Simulator::totalBoatsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Simulator::collisionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Simulator::totalScoreChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Simulator::starsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void Simulator::logMessage(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void Simulator::commandExecuted(Command * _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2);
}

// SIGNAL 12
void Simulator::boatStartedCrossing(Boat * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void Simulator::boatFinishedCrossing(Boat * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void Simulator::collisionDetected(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2);
}

// SIGNAL 15
void Simulator::simulationFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}
QT_WARNING_POP
