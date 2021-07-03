cmake_minimum_required(VERSION 3.18.4)

project(linked_list)

set(LL_SRC
	src/linked_list.c
)

add_library(linked_list ${LL_SRC})

target_include_directories(linked_list
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
)
