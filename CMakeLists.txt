cmake_minimum_required(VERSION 3.12)
project(simnel VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(EXECUTABLE_NAME "d${PROJECT_NAME}"
  CACHE STRING "Executable's name")

add_custom_target(link_server ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME} ${CMAKE_BINARY_DIR}/dserver)

add_custom_target(link_client ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME} ${CMAKE_BINARY_DIR}/dclient)

find_package(Threads REQUIRED)

add_subdirectory(src)
