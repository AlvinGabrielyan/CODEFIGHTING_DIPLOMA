/****************************************************************************
** Meta object code from reading C++ file 'beesimulator.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../beesimulator.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'beesimulator.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12BeeSimulatorE_t {};
} // unnamed namespace

template <> constexpr inline auto BeeSimulator::qt_create_metaobjectdata<qt_meta_tag_ZN12BeeSimulatorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BeeSimulator",
        "currentTimeChanged",
        "",
        "isRunningChanged",
        "speedChanged",
        "completedTripsChanged",
        "totalBeesChanged",
        "totalNectarCollectedChanged",
        "logMessage",
        "message",
        "type",
        "commandExecuted",
        "Command*",
        "command",
        "success",
        "beeStartedFlying",
        "Bee*",
        "bee",
        "beeFinishedFlying",
        "simulationFinished",
        "update",
        "addBee",
        "getBee",
        "id",
        "getAllBees",
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
        "addHoneycomb",
        "capacity",
        "getHoneycombInfo",
        "HoneycombInfo",
        "getAllHoneycombIds",
        "getCommands",
        "currentTime",
        "isRunning",
        "speed",
        "completedTrips",
        "totalBees",
        "totalNectarCollected"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentTimeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRunningChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'speedChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'completedTripsChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'totalBeesChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'totalNectarCollectedChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 },
        }}),
        // Signal 'commandExecuted'
        QtMocHelpers::SignalData<void(Command *, bool)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 }, { QMetaType::Bool, 14 },
        }}),
        // Signal 'beeStartedFlying'
        QtMocHelpers::SignalData<void(Bee *)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'beeFinishedFlying'
        QtMocHelpers::SignalData<void(Bee *)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'simulationFinished'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'update'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'addBee'
        QtMocHelpers::MethodData<void(Bee *)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Method 'getBee'
        QtMocHelpers::MethodData<Bee *(const QString &)>(22, 2, QMC::AccessPublic, 0x80000000 | 16, {{
            { QMetaType::QString, 23 },
        }}),
        // Method 'getAllBees'
        QtMocHelpers::MethodData<QList<QObject*>()>(24, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'loadScript'
        QtMocHelpers::MethodData<bool(const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 27 },
        }}),
        // Method 'getErrors'
        QtMocHelpers::MethodData<QStringList() const>(28, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'getWarnings'
        QtMocHelpers::MethodData<QStringList() const>(29, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'start'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stop'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reset'
        QtMocHelpers::MethodData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'step'
        QtMocHelpers::MethodData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addHoneycomb'
        QtMocHelpers::MethodData<void(const QString &, double)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 }, { QMetaType::Double, 36 },
        }}),
        // Method 'getHoneycombInfo'
        QtMocHelpers::MethodData<HoneycombInfo(const QString &)>(37, 2, QMC::AccessPublic, 0x80000000 | 38, {{
            { QMetaType::QString, 23 },
        }}),
        // Method 'getAllHoneycombIds'
        QtMocHelpers::MethodData<QStringList()>(39, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'getCommands'
        QtMocHelpers::MethodData<void() const>(40, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentTime'
        QtMocHelpers::PropertyData<double>(41, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(42, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'speed'
        QtMocHelpers::PropertyData<double>(43, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'completedTrips'
        QtMocHelpers::PropertyData<int>(44, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'totalBees'
        QtMocHelpers::PropertyData<int>(45, QMetaType::Int, QMC::DefaultPropertyFlags, 4),
        // property 'totalNectarCollected'
        QtMocHelpers::PropertyData<double>(46, QMetaType::Double, QMC::DefaultPropertyFlags, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BeeSimulator, qt_meta_tag_ZN12BeeSimulatorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BeeSimulator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BeeSimulatorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BeeSimulatorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12BeeSimulatorE_t>.metaTypes,
    nullptr
} };

void BeeSimulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BeeSimulator *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentTimeChanged(); break;
        case 1: _t->isRunningChanged(); break;
        case 2: _t->speedChanged(); break;
        case 3: _t->completedTripsChanged(); break;
        case 4: _t->totalBeesChanged(); break;
        case 5: _t->totalNectarCollectedChanged(); break;
        case 6: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->commandExecuted((*reinterpret_cast<std::add_pointer_t<Command*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 8: _t->beeStartedFlying((*reinterpret_cast<std::add_pointer_t<Bee*>>(_a[1]))); break;
        case 9: _t->beeFinishedFlying((*reinterpret_cast<std::add_pointer_t<Bee*>>(_a[1]))); break;
        case 10: _t->simulationFinished(); break;
        case 11: _t->update(); break;
        case 12: _t->addBee((*reinterpret_cast<std::add_pointer_t<Bee*>>(_a[1]))); break;
        case 13: { Bee* _r = _t->getBee((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<Bee**>(_a[0]) = std::move(_r); }  break;
        case 14: { QList<QObject*> _r = _t->getAllBees();
            if (_a[0]) *reinterpret_cast<QList<QObject*>*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->loadScript((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { QStringList _r = _t->getErrors();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 17: { QStringList _r = _t->getWarnings();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->start(); break;
        case 19: _t->pause(); break;
        case 20: _t->stop(); break;
        case 21: _t->reset(); break;
        case 22: _t->step(); break;
        case 23: _t->addHoneycomb((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 24: { HoneycombInfo _r = _t->getHoneycombInfo((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<HoneycombInfo*>(_a[0]) = std::move(_r); }  break;
        case 25: { QStringList _r = _t->getAllHoneycombIds();
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 26: _t->getCommands(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Command* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Bee* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Bee* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Bee* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::currentTimeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::isRunningChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::speedChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::completedTripsChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::totalBeesChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::totalNectarCollectedChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)(const QString & , const QString & )>(_a, &BeeSimulator::logMessage, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)(Command * , bool )>(_a, &BeeSimulator::commandExecuted, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)(Bee * )>(_a, &BeeSimulator::beeStartedFlying, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)(Bee * )>(_a, &BeeSimulator::beeFinishedFlying, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (BeeSimulator::*)()>(_a, &BeeSimulator::simulationFinished, 10))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->currentTime(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->speed(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->completedTrips(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->totalBees(); break;
        case 5: *reinterpret_cast<double*>(_v) = _t->totalNectarCollected(); break;
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

const QMetaObject *BeeSimulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BeeSimulator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12BeeSimulatorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BeeSimulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
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
void BeeSimulator::currentTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BeeSimulator::isRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BeeSimulator::speedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BeeSimulator::completedTripsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BeeSimulator::totalBeesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BeeSimulator::totalNectarCollectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BeeSimulator::logMessage(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void BeeSimulator::commandExecuted(Command * _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}

// SIGNAL 8
void BeeSimulator::beeStartedFlying(Bee * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void BeeSimulator::beeFinishedFlying(Bee * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void BeeSimulator::simulationFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
