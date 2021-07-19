add_subdirectory(instruments)

set(controllers_src_files
  bank.cxx
  controllers.cxx
  instrument_select.cxx
  last_step.cxx
  layout.cxx
  page.cxx
  part.cxx
  play_pause.cxx
  ppqn.cxx
  sequence.cxx
  shift.cxx
  step.cxx)

add_library(controllers ${conrtrollers_src_files})

target_link_libraries(
  controllers
  instrument_controllers)
