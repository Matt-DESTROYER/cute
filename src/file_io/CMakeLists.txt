set(FILE_IO_SRC
	file_io.c
)
add_library(file_io STATIC ${FILE_IO_SRC})
target_include_directories(file_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(file_io PRIVATE tools)

