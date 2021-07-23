# engine src files
set(
  oprlib_engine_src
  engine.cpp)

# add engine library
add_library(oprlib_engine ${oprlib_engine_src})

# include headers and make all library users include also
target_include_directories(oprlib_engine PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link engine to oprlib
target_link_libraries(oprlib INTERFACE oprlib_engine)
