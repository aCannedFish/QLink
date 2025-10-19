cmake_minimum_required(VERSION 3.5)
project(QLink)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Multimedia Test REQUIRED)

set(SOURCES
    block.cpp
    duomode.cpp
    item.cpp
    load.cpp
    main.cpp
    menu.cpp
    pausemenu.cpp
    player.cpp
    simplemode.cpp
    simpletest.cpp
)

set(HEADERS
    block.h
    duomode.h
    item.h
    load.h
    menu.h
    pausemenu.h
    player.h
    simplemode.h
    simpletest.h
)

set(UIS
    duomode.ui
    menu.ui
    pausemenu.ui
    simplemode.ui
)

set(RESOURCES
    images.qrc
    music.qrc
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Multimedia Qt6::Test)
