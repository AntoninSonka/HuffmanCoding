cmake_minimum_required(VERSION 3.10)
project(HuffmanCoding)

file(GLOB_RECURSE SOURCES "src/*.c")

set(MEMWATCH_SRC "memwatch/memwatch.c")
set(MEMWATCH_HEADER "memwatch/memwatch.h")

add_definitions(-DMEMWATCH -DMW_STDIO)

include_directories(memwatch)

add_executable(main ${SOURCES} ${MEMWATCH_SRC})
