cmake_minimum_required(VERSION 3.10)

project(t1x-server)

set(SEPARATOR "---------------------")

# -m32:     32-bit
# -fpic:    Position-independent code
# -Wall:    Enable all warnings
# -fvisibility=hidden: Hide symbols
set(CMAKE_CXX_FLAGS "-m32 -fpic -Wall -fvisibility=hidden")
set(CMAKE_SHARED_LINKER_FLAGS "-m32")
set(CMAKE_LIBRARY_PATH "/lib32" "/usr/lib32" "/usr/lib/i386-linux-gnu" ${CMAKE_LIBRARY_PATH})

# Options
option(DEBUG_BUILD "Include debug info, no optimization" OFF)

message(STATUS "${SEPARATOR}")
message(STATUS "Debug: ${DEBUG_BUILD}")
message(STATUS "${SEPARATOR}")

# -g:       Include debug info
# -ggdb:    Debug info for gdb
# -O2:      Moderate optimization
if(DEBUG_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
else()
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/hook.cpp
    ${CMAKE_SOURCE_DIR}/src/iw1x.cpp
)

add_library(iw1x SHARED ${SOURCES})
target_precompile_headers(iw1x PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.h)

# Output properties
set_target_properties(iw1x PROPERTIES
    OUTPUT_NAME "t1x"
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Linked libraries
target_link_libraries(iw1x
    dl
    pthread
)
