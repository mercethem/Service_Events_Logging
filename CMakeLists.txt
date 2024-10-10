cmake_minimum_required(VERSION 3.29)
project(Service_Events_Logging)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Service_Events_Logging
        Command.cpp
        Command.h
        File.cpp
        File.h
        Logging.cpp
        Logging.h
        main.cpp
        Service.cpp
        Service.h
        Control_Panel.cpp
        Control_Panel.h
        Base_Bussiness.cpp
        Base_Bussiness.h
        Error_Handling.cpp
        Error_Handling.h)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
