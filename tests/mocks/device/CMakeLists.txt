# module src files
set(
  mocks_src
  midi.hpp
  midi.cpp)

# add modules library
add_library(mock-devices ${mocks_src})

# include headers and make all library users include also
target_include_directories(mock-devices PUBLIC ${PROJECT_SOURCE_DIR}/include)


# link device mocks to mocks lib
target_link_libraries(mocks INTERFACE mock-devices)
