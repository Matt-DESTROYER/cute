set(TOOLS_SRC
	tools.c
)
add_library(tools STATIC ${TOOLS_SRC})
target_include_directories(tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

