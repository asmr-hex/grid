set(
  layout_types_src_files
  layout.cxx
  section.cxx)

set(
  layout_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/names
  ${CMAKE_CURRENT_LIST_DIR}/layout
  ${CMAKE_CURRENT_LIST_DIR}/section)

add_library(
  layout_types
  ${layout_types_src_files}
  ${layout_types_src_files})

# add alias library for namespacing
add_library(types::grid::layout ALIAS layout_types)
