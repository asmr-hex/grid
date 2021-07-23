set(sequence_types_src_files
  ${CMAKE_CURRENT_LIST_DIR}/sequence.cxx)

set(sequence_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/sequence.hpp)

add_library(sequence_types ${sequence_types_src_files} ${sequence_types_header_files})

target_include_directories(
  sequence_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
