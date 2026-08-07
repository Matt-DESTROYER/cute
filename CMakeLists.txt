cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(cute C)

add_subdirectory(src/tools)
add_subdirectory(src/file_io)
add_subdirectory(src/ini)

set(SRC src/main.c src/new.c)
add_executable(cute ${SRC})

target_link_libraries(cute PRIVATE tools)
target_link_libraries(cute PRIVATE file_io)
target_link_libraries(cute PRIVATE ini)

if(MSVC)
	target_compile_options(cute PRIVATE /W4 /WX /O2)
else()
	target_compile_options(cute PRIVATE -Wall -Werror -O2)
endif()

