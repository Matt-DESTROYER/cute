cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(cute C)

add_subdirectory(src/tools)
add_subdirectory(src/file_io)
add_subdirectory(src/network)
add_subdirectory(src/ini)

set(SRC
	src/main.c
	src/new.c
	src/add.c
	src/clean.c
	src/build.c
)
add_executable(cute ${SRC})

target_link_libraries(cute PRIVATE tools file_io network ini)

if(MSVC)
	target_compile_options(cute PRIVATE
		/W4 /WX
		$<$<CONFIG:Release>:/O2>
		$<$<CONFIG:Debug>:/ZI;/Od>
	)
else()
	target_compile_options(cute PRIVATE
		-Wall -Werror
		$<$<CONFIG:Release>:-O2>
		$<$<CONFIG:Debug>:-g;-O0>
	)
endif()

