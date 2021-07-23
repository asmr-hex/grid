add_subdirectory(clock)
add_subdirectory(grid)
add_subdirectory(midi)

set(io_src_files io.cxx)
add_library(io ${io_src_files})

target_link_libraries(
  io
  clock
  grid
  midi)
