cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)

project(w11_client C)

file(GLOB_RECURSE SOURCES "*.c" "*.h")

include_directories("src")

file(GLOB_RECURSE SOURCES "src/**/*.c" "src/*.c")

add_executable(w11_client ${SOURCES})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME "client"
)
