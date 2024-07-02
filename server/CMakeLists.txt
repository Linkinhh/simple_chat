cmake_minimum_required(VERSION 3.23)
project(server C)

add_executable(${PROJECT_NAME} src/socketServer.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
    "include/*.h"
)

target_link_libraries(${PROJECT_NAME}
    util
)