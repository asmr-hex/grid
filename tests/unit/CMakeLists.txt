# add all unit test src code
set(unittests_src
  io/test.cpp)

# add main unit test executable (main.cpp defines the Catch2 config)
add_executable(unittests main.cpp ${unittests_src})

# use c++ 17 in tests
target_compile_features(unittests PRIVATE cxx_std_17)

# link libraries
target_link_libraries(unittests PRIVATE Catch2::Catch2) # add anemone

# TODO set include directory (maybe done in top level)???

# auto register all test cases with CTest
catch_discover_tests(unittests)
