/**
 * @file   plugins/instruments/er1/midi.hpp
 * @brief  ER1 Midi Controller
 * @author coco
 * @date   2020-04-01
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_ER1_CONTROLLERS_MIDI_H
#define PLUGINS_INSTRUMENTS_ER1_CONTROLLERS_MIDI_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


namespace ER1 {

  // forward declaration
  class ER1;

  /// @brief A controller for controlling the ER1.
  ///
  class MidiController {
  public:

    MidiController(std::shared_ptr<IO>,
                   std::shared_ptr<State>,
                   std::shared_ptr<ER1>);
  };
};

#endif
