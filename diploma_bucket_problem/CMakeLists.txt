cmake_minimum_required(VERSION 3.16)

project(diploma_bucket_problem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    interpreter.cpp
    interpreter.h
    problem_definition.cpp
    problem_definition.h
    problem_registry.cpp
    problem_registry.h
    inputwindow.cpp
    inputwindow.h
    resultsaver.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(diploma_bucket_problem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        inputwindow.h
        inputwindow.cpp


    )
else()
    if(ANDROID)
        add_library(diploma_bucket_problem SHARED ${PROJECT_SOURCES})
    else()
        add_executable(diploma_bucket_problem ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(diploma_bucket_problem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.diploma_bucket_problem)
endif()
set_target_properties(diploma_bucket_problem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS diploma_bucket_problem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(diploma_bucket_problem)
endif()
