cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(moba-illumination VERSION 1.0.0)

configure_file(config.h.in config.h)

add_executable(
    moba-illumination

    src/main.cpp
    src/monitor.cpp
    src/msghandler.cpp
)

install(TARGETS moba-illumination)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
target_link_libraries(moba-illumination PkgConfig::deps)

find_path(GLIB_INCLUDE_DIR NAMES glib.h PATH_SUFFIXES glib-2.0)

target_include_directories(moba-illumination PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(moba-illumination ncurses)
target_link_libraries(moba-illumination mobacommon)
target_link_libraries(moba-illumination z)
target_link_libraries(moba-illumination ${CMAKE_SOURCE_DIR}/modules/lib-msghandling/libmoba-lib-msghandling.a)

include_directories(${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src)
include(GNUInstallDirs)