set(
  grid_device_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/coordinates.hpp
  ${CMAKE_CURRENT_LIST_DIR}/event.hpp)

add_library(grid_device_types ${grid_device_types_header_files})

target_include_directories(
  grid_device_types
  ${CMAKE_CURRENT_LIST_DIR})
