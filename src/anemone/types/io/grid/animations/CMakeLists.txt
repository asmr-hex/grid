set(animation_types_src_files periodic_waveforms.cxx)
set(animation_types_header_files ${CMAKE_CURRENT_LIST_DIR}/periodic_waveforms.hpp)

add_library(
  animation_types
  ${animation_types_src_files}
  ${animation_types_header_files})

target_include_directories(
  animation_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
