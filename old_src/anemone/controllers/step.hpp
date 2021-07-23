/**
 * @file   controllers/step.hpp
 * @brief  Step Controller
 * @author coco
 * @date   2020-02-25
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_STEP_H
#define ANEMONE_CONTROLLERS_STEP_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for updating playing part steps.
///
class StepController {
public:
  StepController(std::shared_ptr<IO>, std::shared_ptr<State>);
private:
  /// @brief previous values
  struct {
    page_idx_t playing_page = 0;
  } previous;
};

#endif
