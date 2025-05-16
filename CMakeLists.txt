cmake_minimum_required(VERSION 3.16)
project(cwus)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets DBus Quick Gui WaylandCompositor REQUIRED)
find_package(LayerShellQt REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
qt_add_resources(QML_RESOURCES resources.qrc)

add_executable(cwus
    main.cpp
    controller.cpp
    loader.cpp
    #controller.h
    ${QML_RESOURCES}
)

target_link_libraries(cwus PRIVATE
    Qt6::Gui
    Qt6::Quick
    Qt6::DBus
    Qt6::WaylandCompositor
    LayerShellQt::Interface
)
