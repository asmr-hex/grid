/**
 * @file   controllers/layout.hpp
 * @brief  Layout Controller
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_LAYOUT_H
#define ANEMONE_CONTROLLERS_LAYOUT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An conntroller for switching/managing layouts.
///
class LayoutController {
public:
  LayoutController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
