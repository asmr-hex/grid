set(test
  io/test.cpp)

# add main unit test executable
add_executable(unittests main.cpp ${tests})

# use c++ 17 in tests
target_compile_features(unittests PRIVATE cxx_std_17)

# link libraries
target_link_libraries(unittests PRIVATE anemone Catch2::Catch2)

# TODO set include directory (maybe done in top level)

add_test(NAME test COMMAND unittests)
