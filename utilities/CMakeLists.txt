cmake_minimum_required(VERSION 3.23)
project(socketUtil C)
add_library(util STATIC src/socketUtil.c)
target_include_directories(util PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
)