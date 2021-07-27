# add interface library
add_library(opr INTERFACE)

# add all sub-libraries
add_subdirectory(engine)
add_subdirectory(io)
add_subdirectory(modules)

# include headers and make all library users include also
# target_include_directories(oprlib INTERFACE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(opr INTERFACE cxx_std_17)
