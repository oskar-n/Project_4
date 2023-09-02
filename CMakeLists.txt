cmake_minimum_required(VERSION 3.8)
project(projekt_4)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Setting DEBUG flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Setting RELEASE flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR "SFML/include")

set(SFML_LIBRARY_DIR "SFML/lib")

set(SFML_LIBRARIES sfml-graphics sfml-audio sfml-window sfml-system)

include_directories(${SFML_INCLUDE_DIR})

link_directories(${SFML_LIBRARY_DIR})

set(SOURCES main.cpp)

add_executable(projekt_4 ${SOURCES})


target_link_libraries(projekt_4 ${SFML_LIBRARIES})
