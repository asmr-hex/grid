/**
 * @file   plugins/instruments/microgranny/midi.hpp
 * @brief  MicroGranny Midi Controller
 * @author coco
 * @date   2020-03-29
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLERS_MIDI_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLERS_MIDI_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


namespace MicroGranny {

  // forward declaration
  class MicroGranny;

  /// @brief A controller for controlling the MicroGranny.
  ///
  class MidiController {
  public:

    MidiController(std::shared_ptr<IO>,
                   std::shared_ptr<State>,
                   std::shared_ptr<MicroGranny>);
  };
};

#endif
