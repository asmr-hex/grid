# download Catch2 testing framework
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

# add Catch2 cmake modules to cmake module path
list(APPEND CMAKE_MODULE_PATH ${catch_SOURCE_DIR}/contrib)

# include Catch CMake module
include(Catch)

# TODO download trompeloeil.hpp?

# include test mocks
add_subdirectory(mocks)

# include unit test source code
add_subdirectory(unit)

# TODO include integration test source code
#add_subdirectory(integration)

# include current directory for unittests (for access to mock headers)
target_include_directories(unittests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
