cmake_minimum_required(VERSION 3.16)

project(DiplomaLauncher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# NOTE: mainwindow.ui removed — the UI is built entirely in C++ code.
# Keeping the .ui file caused uic/linker mismatches. If you re-add it,
# you must also add Ui::MainWindow *ui; to mainwindow.h and call
# ui->setupUi(this) in the constructor.
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiplomaLauncher
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resultsaver.h
    )
else()
    if(ANDROID)
        add_library(DiplomaLauncher SHARED ${PROJECT_SOURCES})
    else()
        add_executable(DiplomaLauncher ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(DiplomaLauncher PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DiplomaLauncher)
endif()
set_target_properties(DiplomaLauncher PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DiplomaLauncher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DiplomaLauncher)
endif()
