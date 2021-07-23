# add interface library
add_library(oprlib INTERFACE)

# add all library "sub-modules"
add_subdirectory(engine)
add_subdirectory(io)

# include headers and make all library users include also
target_include_directories(oprlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_features(oprlib PUBLIC cxx_std_17)
