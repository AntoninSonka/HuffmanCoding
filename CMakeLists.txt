cmake_minimum_required(VERSION 3.10)
project(HuffmanCoding)

file(GLOB_RECURSE SOURCES "src/*.c")

set(MEMWATCH_SRC "memwatch/memwatch.c")
set(MEMWATCH_HEADER "memwatch/memwatch.h")

add_definitions(-DMEMWATCH -DMW_STDIO)

include_directories(memwatch)

add_compile_options(-Wall)

set_source_files_properties(memwatch.c PROPERTIES COMPILE_FLAGS "-w")

add_executable(main ${SOURCES} ${MEMWATCH_SRC})

target_link_libraries(main m)
