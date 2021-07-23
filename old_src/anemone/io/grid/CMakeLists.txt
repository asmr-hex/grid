add_subdirectory(animation)
add_subdirectory(device)

set(grid_src_files grid.cxx)
add_library(grid ${grid_src_files})

target_link_libraries(
  grid
  grid_animation
  grid_device)
