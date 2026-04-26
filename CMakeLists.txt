cmake_minimum_required(VERSION "3.25")
project(radical
    VERSION 0.0.1
    LANGUAGES C
)

# C99, classic stuff. avoid compiler-specifics where you can!
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# very helpful for IDEs that can use it
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB RAD_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

file(GLOB RAD_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

string(TIMESTAMP BUILD_TIME "%Y-%m-%dT%H:%M:%SZ" UTC)

add_library(radical SHARED
    ${RAD_SOURCES}
    ${RAD_HEADERS}
)

target_include_directories(radical PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(radical PUBLIC
    RAD_BUILD_TIME=${BUILD_TIME}
)
