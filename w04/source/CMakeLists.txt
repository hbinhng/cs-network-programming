cmake_minimum_required(VERSION 3.0)

project(NguyenHaiBinh_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(NguyenHaiBinh_4
  src/main.c
  src/common.h
  src/common.c
  src/client.h
  src/client.c
  src/server.h
  src/server.c
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    OUTPUT_NAME "main"
)