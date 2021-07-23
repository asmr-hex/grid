/**
 * @file   controllers/part.hpp
 * @brief  Part Controller
 * @author coco
 * @date   2020-02-23
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_PART_H
#define ANEMONE_CONTROLLERS_PART_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for selecting parts.
///
class PartController {
public:
  PartController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
