cmake_minimum_required(VERSION 3.6.0)
project(hoymilesClient VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(inc inc/libmodbus inc/hoymiles inc/hoymiles/portParameters inc/hoymiles/sunspecParameters)

file(GLOB SOURCES src/*.cpp src/libmodbus/*.c src/hoymiles/*.cpp src/hoymiles/portParameters/*.cpp src/hoymiles/sunspecParameters/*.cpp)

add_executable(hoymilesClient_exec ${SOURCES})

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/libmodbus/config.h)

if(WIN32)
    # find_library(ws2_32_library NAMES libws2_32.a HINTS /usr/x86_64-w64-mingw32/lib)
    # target_link_libraries(hoymilesClient_exec "/usr/x86_64-w64-mingw32/lib/libws2_32.a")
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -static -static-libgcc -static-libstdc++")
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -static-libgcc -static-libstdc++ -lwsock32 -lws2_32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif(WIN32)