cmake_minimum_required(VERSION 3.20)


set(CMAKE_C_COMPILER "C:\\msys64\\mingw64\\bin\\gcc.exe")
set(CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\g++.exe")
project(har_parser_app LANGUAGES CXX)


# --- Стандарт C++
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --- Флаги для MinGW
if (MINGW)
    message(STATUS "Compiling with MinGW / GCC")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DNDEBUG -march=native")
endif()

# --- Исходники
add_executable(har_parser_app
    main.cpp
    parser/Parser.cpp
    parser/Parser.hpp
    include/Entry.hpp
    include/Occurrence.hpp
    include/Variable.hpp
    include/Indexes.hpp
    lib/simdjson/simdjson.cpp
)


# Подключаем include/
target_include_directories(har_parser_app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/parser
    ${CMAKE_SOURCE_DIR}/lib/simdjson
)

# Проверка что simdjson.h есть
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/simdjson/simdjson.h")
    message(FATAL_ERROR "simdjson.h не найден! Скачай simdjson и положи в lib/simdjson/include")
endif()
 message(Good job "simdjson.h найден!")