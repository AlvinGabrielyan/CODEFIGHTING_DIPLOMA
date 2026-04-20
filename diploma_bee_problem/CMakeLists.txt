cmake_minimum_required(VERSION 3.16)

project(diploma_bee_problem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ՈՒՂՂՈՒՄ: ավելացված Sql module (resultsaver.h-ի համար)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        beesimulatorwindow.cpp
        beesimulatorwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(diploma_bee_problem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        bee.h bee.cpp
        beesimulator.h beesimulator.cpp
        beesystem.h beesystem.cpp
        command.h command.cpp
        interpreter.h interpreter.cpp
        honeycomb.h honeycomb.cpp
        flower.h flower.cpp
        resultsaver.h
    )
else()
    if(ANDROID)
        add_library(diploma_bee_problem SHARED ${PROJECT_SOURCES})
    else()
        add_executable(diploma_bee_problem ${PROJECT_SOURCES})
    endif()
endif()

# ՈՒՂՂՈՒՄ: ավելացված Qt::Sql
target_link_libraries(diploma_bee_problem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.diploma_bee_problem)
endif()
set_target_properties(diploma_bee_problem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS diploma_bee_problem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(diploma_bee_problem)
endif()
