add_subdirectory(page)
add_subdirectory(part)
add_subdirectory(sequence)
add_subdirectory(step)

set(instrument_types_src_files
  instrument.cxx)

set(instrument_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/instrument.hpp
  ${CMAKE_CURRENT_LIST_DIR}/names.hpp)

add_library(instrument_types
  ${instrument_types_src_files}
  ${instrument_types_header_files})

target_include_directories(
  instrument_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  instrument_types
  page_types
  part_types
  sequence_types
  step_types)
