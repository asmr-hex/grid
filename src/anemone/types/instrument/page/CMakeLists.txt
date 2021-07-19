set(page_types_header_files
  ${CMAKE_CURRENT_LIST_DIR}/page.hpp)

add_library(page_types ${page_types_header_files})

target_include_directories(
  page_types
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR})
