/**
 * @file   plugins/instruments/microgranny/controllers/pads.hpp
 * @brief  MicroGranny Pads Controller
 * @author coco
 * @date   2020-03-30
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLERS_PADS_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_CONTROLLERS_PADS_H

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
  class PadsController {
  public:

    PadsController(std::shared_ptr<IO>,
                   std::shared_ptr<State>,
                   std::shared_ptr<MicroGranny>);
  };
};

#endif
