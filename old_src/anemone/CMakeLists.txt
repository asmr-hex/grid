add_subdirectory(io)
add_subdirectory(state)
add_subdirectory(controllers)
add_subdirectory(types)
add_subdirectory(ui)
add_subdirectory(util)
add_subdirectory(plugins)

set(anemone_interface_src_files anemone.cxx)
add_library(anemone_interface ${anemone_interface_src_files})

target_link_libraries(
  anemone_interface
  io
  state
  controllers
  types
  ui
  util
  plugins)
