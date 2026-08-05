set(INI_SRC
	ini.c
	djb2.c
	hashmap.c
)
add_library(ini STATIC ${INI_SRC})
target_include_directories(ini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ini PRIVATE file_io)

