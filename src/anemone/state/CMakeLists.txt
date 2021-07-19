add_subdirectory(controls)
add_subdirectory(instruments)
add_subdirectory(layouts)

set(state_src_files state.cxx)
add_library(state ${state_src_files})

target_link_libraries(
  state
  controls_state
  instruments_state
  layouts_state)
