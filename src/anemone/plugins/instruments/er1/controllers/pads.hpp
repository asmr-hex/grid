/**
 * @file   plugins/instruments/er1/pads.hpp
 * @brief  ER1 Pads Controller
 * @author coco
 * @date   2020-04-02
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_ER1_CONTROLLERS_PADS_H
#define PLUGINS_INSTRUMENTS_ER1_CONTROLLERS_PADS_H

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
  class PadsController {
  public:

    PadsController(std::shared_ptr<IO>,
                   std::shared_ptr<State>,
                   std::shared_ptr<ER1>);
  };
};

#endif
