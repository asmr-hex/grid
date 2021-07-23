# engine src files
set(
  oprlib_engine_src
  engine.cpp)

# add engine library
add_library(oprlib_engine ${oprlib_engine_src})

# link engine to oprlib
target_link_libraries(oprlib PUBLIC oprlib_engine)
