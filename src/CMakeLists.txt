cmake_minimum_required(VERSION 3.5)

project(trading VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cc
        view/mainwindow.cc
        view/mainwindow.h
        view/mainwindow.ui
        view/approx_dialog.cc
        view/approx_dialog.h
        view/interpol_dialog.h
        view/interpol_dialog.cc
        other/qcustomplot.cpp
        other/qcustomplot.h
        model/cubic_splines.cc
        model/newton.cc
        model/least_squares.cc
        model/gauss.cc
        stonks.png
        )

set(QRC_SOURCES
    icon.qrc
)

if (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE stonks.png)
  set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/stonks.png")
  set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(trading
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES} ${app_icon_macos}
    )
else()
    add_executable(trading
        ${PROJECT_SOURCES} ${app_icon_macos}
    )
endif()

target_link_libraries(trading PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(trading PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(trading)
endif()
