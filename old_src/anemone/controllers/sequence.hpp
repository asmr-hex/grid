/**
 * @file   controllers/sequence.hpp
 * @brief  Sequence Controller
 * @author coco
 * @date   2020-03-03
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_SEQUENCE_H
#define ANEMONE_CONTROLLERS_SEQUENCE_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for updating sequence steps.
///
class SequenceController {
public:
  SequenceController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
