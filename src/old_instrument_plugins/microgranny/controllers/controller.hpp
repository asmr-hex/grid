/**
 * @file   plugins/instruments/microgranny/controllers/controller.hpp
 * @brief  MicroGranny Controller
 * @author coco
 * @date   2020-03-28
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLER_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLER_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/controllers/controller.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/midi.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/pads.hpp"


namespace MicroGranny {

  // forward declaration
  class MicroGranny;

  /// @brief A controller for controlling the MicroGranny.
  ///
  class Controller : public StateController {
  public:
    /// @brief midi controller.
    std::unique_ptr<MidiController> midi;

    /// @brief pads controller.
    std::unique_ptr<PadsController> pads;

    Controller(std::shared_ptr<IO>,
               std::shared_ptr<State>,
               std::shared_ptr<MicroGranny>);
  };
};

#endif
