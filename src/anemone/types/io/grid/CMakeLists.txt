add_subdirectory(animations)
add_subdirectory(device)
add_subdirectory(layout)

set(grid_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/address.hpp
  ${CMAKE_CURRENT_LIST_DIR}/animation.hpp
  ${CMAKE_CURRENT_LIST_DIR}/event.hpp)

add_library(grid_types $grid_types_header_files)

target_include_directories(
  grid_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  grid_types
  animation_types
  grid_device_types
  layout_types)
