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


namespace ER1 {

  // forward declaration
  class ER1;

  /// @brief A controller for controlling the ER1.
  ///
  class Controller : public StateController {
  public:
    Controller(std::shared_ptr<IO>,
               std::shared_ptr<State>,
               std::shared_ptr<ER1>);
  };
};

#endif
