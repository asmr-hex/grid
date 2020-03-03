/**
 * @file   controllers/page.hpp
 * @brief  Page Controller
 * @author coco
 * @date   2020-03-02
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_PAGE_H
#define ANEMONE_CONTROLLERS_PAGE_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for selecting pages.
///
class PageController {
public:
  PageController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
