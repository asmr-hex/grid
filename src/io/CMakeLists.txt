# io src files
set(
  oprlib_io_src
  io.cpp)

# add io library
add_library(oprlib_io ${oprlib_io_src})

# link io to oprlib
target_link_libraries(oprlib PUBLIC oprlib_io)
