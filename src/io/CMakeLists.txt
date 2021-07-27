# io src files
set(
  opr_io_src
  io.cpp)

# add io library
add_library(opr-io ${opr_io_src})

# include headers and make all library users include also
target_include_directories(opr-io PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link io to oprlib
target_link_libraries(opr INTERFACE opr-io)
