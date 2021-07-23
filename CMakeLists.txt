cmake_minimum_required(VERSION 3.14...3.21)

project(
  Operator
  VERSION 3.0
  DESCRIPTION "multimodal, tentacled, sonic controller"
  LANGUAGES CXX)

# actions only to be performed in the current project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # enable testing
  include(CTest)

  # TODO add Doxygen support
  # see https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/CMakeLists.txt#L29
endif()

# enable FetchContent for downloading dependencies
include(FetchContent)

# TODO fetch dependencies

# add library source code
add_subdirectory(src)

# TODO add main executable source code
#add_subdirectory(app)

# tests are only available in the current project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
