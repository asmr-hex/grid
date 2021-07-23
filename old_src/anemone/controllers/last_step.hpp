/**
 * @file   controllers/last_step.hpp
 * @brief  Last Step Controller
 * @author coco
 * @date   2020-03-11
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_LAST_STEP_H
#define ANEMONE_CONTROLLERS_LAST_STEP_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for updating the last step of a part
///
class LastStepController {
public:
  LastStepController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
