# module src files
set(
  opr_impl_src
  midi.cpp)

# add modules library
add_library(opr-impl ${opr_impl_src})

# include headers and make all library users include also
target_include_directories(opr-impl PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link modules to libopr
target_link_libraries(opr INTERFACE opr-impl)
