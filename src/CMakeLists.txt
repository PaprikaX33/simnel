set(MAIN_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/socket.c
  CACHE PATH "Main source files")

set(MAIN_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  CACHE PATH "Main header include directories")

add_subdirectory(include)

add_executable(${EXECUTABLE_NAME}
  ${MAIN_SOURCE_FILES}
  ${MAIN_HEADER_FILES}
  )

target_include_directories(${EXECUTABLE_NAME}
  PRIVATE
  ${MAIN_INCLUDE_DIR}
  )
target_link_libraries(${EXECUTABLE_NAME}
  PRIVATE
  Threads::Threads
  m)
target_compile_features(${EXECUTABLE_NAME} PRIVATE
  c_std_11
  c_static_assert
  c_variadic_macros
  c_function_prototypes)
target_compile_options(${EXECUTABLE_NAME} PRIVATE $<$<CONFIG:DEBUG>:
  -Werror
  -g
  -Og
  -Wall
  -Wextra
  -Wshadow
  -Wcast-align
  -Wunused
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wnull-dereference
  -Wdouble-promotion
  -Wformat=2
  -Wduplicated-cond
  -Wduplicated-branches
  -Wlogical-op
  >)
