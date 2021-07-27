# engine src files
set(
  opr_engine_src
  engine.cpp)

# add engine library
add_library(opr-engine ${opr_engine_src})

# include headers and make all library users include also
target_include_directories(opr-engine PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link engine to oprlib
target_link_libraries(opr INTERFACE opr-engine)
