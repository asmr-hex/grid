# module src files
set(
  opr_modules_src
  sequencer.cpp)

# add modules library
add_library(opr-modules ${opr_modules_src})

# include headers and make all library users include also
target_include_directories(opr-modules PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link modules to libopr
target_link_libraries(opr INTERFACE opr-modules)
