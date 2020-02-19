/**
 * @file   controllers/shift.hpp
 * @brief  Shift Controller
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_SHIFT_H
#define ANEMONE_CONTROLLERS_SHIFT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for toggling the shift button.
///
class ShiftController {
public:
  ShiftController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
