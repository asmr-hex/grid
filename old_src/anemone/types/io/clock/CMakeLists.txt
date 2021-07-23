set(clock_types_header_files ${CMAKE_CURRENT_LIST_DIR}/tick.hpp)

add_library(clock_types ${clock_types_header_files})

target_include_directories(
  clock_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
