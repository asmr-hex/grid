add_subdirectory(device)

set(midi_src_files midi.cxx)
add_library(midi ${midi_src_files})

target_link_libraries(
  midi
  midi_device)
