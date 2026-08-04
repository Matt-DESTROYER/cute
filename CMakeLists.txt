cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(cute C)

file(GLOB SRC "src/*.c")

add_executable(cute ${SRC})

if(MSVC)
	target_compile_options(cute PRIVATE /W4 /WX /O2)
else()
	target_compile_options(cute PRIVATE -Wall -Werror -O2)
endif()

