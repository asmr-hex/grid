/**
 * @file   plugins/instruments/er1/controller.hpp
 * @brief  ER1 Controller
 * @author coco
 * @date   2020-03-21
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_ER1_CONTROLLER_H
#define PLUGINS_INSTRUMENTS_ER1_CONTROLLER_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/controllers/controller.hpp"
#include "anemone/plugins/instruments/er1/controllers/midi.hpp"
#include "anemone/plugins/instruments/er1/controllers/pads.hpp"


namespace ER1 {

  // forward declaration
  class ER1;

  /// @brief A controller for controlling the ER1.
  ///
  class Controller : public StateController {
  public:
    /// @brief midi controller.
    std::unique_ptr<MidiController> midi;

    /// @brief pads controller.
    std::unique_ptr<PadsController> pads;

    Controller(std::shared_ptr<IO>,
               std::shared_ptr<State>,
               std::shared_ptr<ER1>);
  };
};

#endif
