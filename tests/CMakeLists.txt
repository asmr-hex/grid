# download Catch2 testing framework
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

# TODO download trompeloeil.hpp

# include unit test source code
add_subdirectory(unit)

# TODO include integration test source code
#add_subdirectory(integration)
