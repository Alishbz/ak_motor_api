
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TEST CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    main.cpp
    ex_apis/serial_port_api.cpp
)

include_directories(ex_apis)
include_directories(t_motor)


add_executable(main ${SOURCES}   "t_motor/serial_crc.h" "t_motor/control.h" "t_motor/mit_parser.h" "mini_tdd.h" "t_motor/ak_motor_container.h")