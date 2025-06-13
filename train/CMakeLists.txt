cmake_minimum_required(VERSION 3.10)
project(train_database C)

set(CMAKE_C_STANDART 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

find_package(Curses REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(include)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(train_db main.c)

target_link_libraries(train_db ${CURSES_LIBRARIES} ${ZLIB_LIBRARIES})
