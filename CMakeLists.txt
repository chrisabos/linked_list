cmake_minimum_required(VERSION 3.18.4)

project(ll_lib)

set(LL_SRC
	src/linked_list.c
)

add_executable(ll_lib ${LL_SRC})

target_include_directories(ll_lib
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)
