set(NETWORK_SRC
	network.c
)

find_package(CURL REQUIRED)

add_library(network STATIC ${NETWORK_SRC})
target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(network PRIVATE tools file_io CURL::libcurl)

