include_directories(inc)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h)

file(GLOB SOURCES src/*.c)

add_library(libmodbus STATIC ${SOURCES})