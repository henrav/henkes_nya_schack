cmake_minimum_required(VERSION 4.0)
project(schack)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)

add_executable(schack main.cpp
        Board.cpp
        Board.h
        GUI.cpp
        GUI.h
        loadMoveAttackPatterns.cpp
        loadMoveAttackPatterns.h
        loadMoveAttackPatterns.h
        Enums.h
        WorkerThread.cpp
        WorkerThread.h

)

target_link_libraries(schack
        PRIVATE
        SFML::Graphics
        SFML::Audio
        ${LUA_LIBRARIES}
)
