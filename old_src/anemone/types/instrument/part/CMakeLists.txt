set(part_types_src_files
  ${CMAKE_CURRENT_LIST_DIR}/part.cxx)

set(part_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/part.hpp)

add_library(part_types ${part_types_src_files} ${part_types_header_files})

target_include_directories(
  part_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
