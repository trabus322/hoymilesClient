cmake_minimum_required(VERSION 3.6.0)
project(hoymilesClient VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include(CTest)
enable_testing()

include_directories(inc inc/libmodbus inc/hoymiles inc/hoymiles/portParameters)

file(GLOB SOURCES src/*.cpp src/libmodbus/*.c src/hoymiles/*.cpp src/hoymiles/portParameters/*.cpp)

add_executable(hoymilesClient_exec ${SOURCES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/libmodbus/config.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
