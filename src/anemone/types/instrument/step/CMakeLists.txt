set(step_types_src_files
  ${CMAKE_CURRENT_LIST_DIR}/step.cxx)

set(step_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/step.hpp)

add_library(step_types ${step_types_src_files} ${step_types_header_files})

target_include_directories(
  step_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
