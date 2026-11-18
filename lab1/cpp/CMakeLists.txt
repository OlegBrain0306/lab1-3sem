cmake_minimum_required(VERSION 3.10)
project(dbms CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dbms
    src/main.cpp
    src/commands.cpp
    src/array.cpp
    src/flist.cpp
    src/dlist.cpp
    src/stack.cpp
    src/queue.cpp
    src/fbt.cpp
)
target_include_directories(dbms PRIVATE include)

if(MSVC)
    target_compile_options(dbms PRIVATE /W4 /utf-8)
else()
    target_compile_options(dbms PRIVATE -Wall -Wextra)
endif()
