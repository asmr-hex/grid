add_subdirectory(clock)
add_subdirectory(grid)
add_subdirectory(midi)

add_library(io_types "")

target_link_libraries(
  io_types
  clock_types
  grid_types
  midi_types)
