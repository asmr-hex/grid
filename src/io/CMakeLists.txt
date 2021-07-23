# io src files
set(
  oprlib_io_src
  io.cpp)

# add io library
add_library(oprlib_io ${oprlib_io_src})

# include headers and make all library users include also
target_include_directories(oprlib_io PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link io to oprlib
target_link_libraries(oprlib INTERFACE oprlib_io)
