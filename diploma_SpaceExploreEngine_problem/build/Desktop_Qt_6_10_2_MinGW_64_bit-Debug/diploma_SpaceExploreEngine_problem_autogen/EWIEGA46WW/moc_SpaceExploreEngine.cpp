/****************************************************************************
** Meta object code from reading C++ file 'SpaceExploreEngine.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../SpaceExploreEngine.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SpaceExploreEngine.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19SpaceExplorerEngineE_t {};
} // unnamed namespace

template <> constexpr inline auto SpaceExplorerEngine::qt_create_metaobjectdata<qt_meta_tag_ZN19SpaceExplorerEngineE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SpaceExplorerEngine",
        "explorersChanged",
        "",
        "scannedCellsChanged",
        "visitedCellsChanged",
        "planetTerritoriesChanged",
        "missionStatusChanged",
        "isRunningChanged",
        "wastedMovesChanged",
        "errorOccurred",
        "error",
        "executeStep",
        "resetMission",
        "startMission",
        "pauseMission",
        "addCommands",
        "explorerId",
        "commands",
        "clearCommands",
        "getCommands",
        "setSpeed",
        "milliseconds",
        "getPathHistory",
        "QVariantList",
        "explorers",
        "scannedCells",
        "visitedCells",
        "planetTerritories",
        "missionStatus",
        "isRunning",
        "gridSize",
        "scannedCount",
        "totalTerritories",
        "wastedMoves"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'explorersChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scannedCellsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'visitedCellsChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'planetTerritoriesChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'missionStatusChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRunningChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'wastedMovesChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Slot 'executeStep'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'resetMission'
        QtMocHelpers::MethodData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startMission'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pauseMission'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addCommands'
        QtMocHelpers::MethodData<void(int, const QStringList &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 }, { QMetaType::QStringList, 17 },
        }}),
        // Method 'clearCommands'
        QtMocHelpers::MethodData<void(int)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Method 'getCommands'
        QtMocHelpers::MethodData<QStringList(int) const>(19, 2, QMC::AccessPublic, QMetaType::QStringList, {{
            { QMetaType::Int, 16 },
        }}),
        // Method 'setSpeed'
        QtMocHelpers::MethodData<void(int)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 },
        }}),
        // Method 'getPathHistory'
        QtMocHelpers::MethodData<QVariantList(int) const>(22, 2, QMC::AccessPublic, 0x80000000 | 23, {{
            { QMetaType::Int, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'explorers'
        QtMocHelpers::PropertyData<QVariantList>(24, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'scannedCells'
        QtMocHelpers::PropertyData<QVariantList>(25, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'visitedCells'
        QtMocHelpers::PropertyData<QVariantList>(26, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
        // property 'planetTerritories'
        QtMocHelpers::PropertyData<QVariantList>(27, 0x80000000 | 23, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 3),
        // property 'missionStatus'
        QtMocHelpers::PropertyData<QString>(28, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(29, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'gridSize'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'scannedCount'
        QtMocHelpers::PropertyData<int>(31, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'totalTerritories'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'wastedMoves'
        QtMocHelpers::PropertyData<int>(33, QMetaType::Int, QMC::DefaultPropertyFlags, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SpaceExplorerEngine, qt_meta_tag_ZN19SpaceExplorerEngineE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SpaceExplorerEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19SpaceExplorerEngineE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19SpaceExplorerEngineE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19SpaceExplorerEngineE_t>.metaTypes,
    nullptr
} };

void SpaceExplorerEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SpaceExplorerEngine *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->explorersChanged(); break;
        case 1: _t->scannedCellsChanged(); break;
        case 2: _t->visitedCellsChanged(); break;
        case 3: _t->planetTerritoriesChanged(); break;
        case 4: _t->missionStatusChanged(); break;
        case 5: _t->isRunningChanged(); break;
        case 6: _t->wastedMovesChanged(); break;
        case 7: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->executeStep(); break;
        case 9: _t->resetMission(); break;
        case 10: _t->startMission(); break;
        case 11: _t->pauseMission(); break;
        case 12: _t->addCommands((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 13: _t->clearCommands((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: { QStringList _r = _t->getCommands((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QStringList*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->setSpeed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: { QVariantList _r = _t->getPathHistory((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::explorersChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::scannedCellsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::visitedCellsChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::planetTerritoriesChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::missionStatusChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::isRunningChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)()>(_a, &SpaceExplorerEngine::wastedMovesChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (SpaceExplorerEngine::*)(const QString & )>(_a, &SpaceExplorerEngine::errorOccurred, 7))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->explorers(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->scannedCells(); break;
        case 2: *reinterpret_cast<QVariantList*>(_v) = _t->visitedCells(); break;
        case 3: *reinterpret_cast<QVariantList*>(_v) = _t->planetTerritories(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->missionStatus(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 6: *reinterpret_cast<int*>(_v) = _t->gridSize(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->scannedCount(); break;
        case 8: *reinterpret_cast<int*>(_v) = _t->totalTerritories(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->wastedMoves(); break;
        default: break;
        }
    }
}

const QMetaObject *SpaceExplorerEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpaceExplorerEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19SpaceExplorerEngineE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SpaceExplorerEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
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
void SpaceExplorerEngine::explorersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SpaceExplorerEngine::scannedCellsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SpaceExplorerEngine::visitedCellsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SpaceExplorerEngine::planetTerritoriesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SpaceExplorerEngine::missionStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SpaceExplorerEngine::isRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SpaceExplorerEngine::wastedMovesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SpaceExplorerEngine::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
