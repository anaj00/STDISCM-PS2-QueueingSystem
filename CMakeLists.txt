cmake_minimum_required(VERSION 3.30)
project(Code)

set(CMAKE_CXX_STANDARD 20)

add_executable(Code main.cpp
        DungeonInstance.cpp
        DungeonInstance.h
        DungeonManager.cpp
        DungeonManager.h
        ConsoleManager.cpp
        ConsoleManager.h
        Utility.h
)
