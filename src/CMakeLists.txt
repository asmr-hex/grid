add_subdirectory(anemone)


set(anemone_src_files main.cxx)
add_executable(anemone ${anemone_src_files})

target_link_libraries(anemone anemone_interface)
