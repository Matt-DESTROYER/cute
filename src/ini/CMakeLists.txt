set(INI_SRC
	ini.c
)
add_library(ini STATIC ${INI_SRC})
target_include_directories(ini PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ini PRIVATE tools file_io)

