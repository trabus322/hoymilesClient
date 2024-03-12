cmake_minimum_required(VERSION 3.6.0)
project(hoymilesClient VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include_directories(inc)

file(GLOB SOURCES src/*.cpp src/*.c)

add_executable(hoymilesClient_exec ${SOURCES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
