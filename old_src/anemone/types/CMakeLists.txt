add_subdirectory(controls)
add_subdirectory(instrument)
add_subdirectory(io)

add_library(types "")

target_link_libraries(
  types
  control_types
  instrument_types
  io_types)
