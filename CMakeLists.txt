cmake_minimum_required(VERSION 3.14)

project(googleQt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Network Xml Sql REQUIRED)
if (${QT_VERSION_MAJOR} GREATER 5)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat REQUIRED)
endif()

add_library(googleQt STATIC)
add_subdirectory(src)

target_include_directories(googleQt PUBLIC src)

target_link_libraries(googleQt
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Gui
)

if (${QT_VERSION_MAJOR} GREATER 5)
    target_link_libraries(googleQt PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
endif()

target_compile_definitions(googleQt PRIVATE _HAS_AUTO_PTR_ETC)

if(${ARD_AUTOTEST} )
    target_compile_definitions(googleQt PRIVATE API_QT_AUTOTEST=1)
    target_compile_definitions(googleQt PRIVATE API_QT_DIAGNOSTICS=1)
endif()
