/**
 * @file   state/selectors/pages.hpp
 * @brief  State Selectors For Part Pages
 * @author coco
 * @date   2020-01-19
 *************************************************/


#ifndef STATE_SELECTORS_PAGES_H
#define STATE_SELECTORS_PAGES_H

#include "anemone/state/root.hpp"
#include "anemone/types.hpp"
#include "anemone/io.hpp"


/// @brief Select page under edit in the currently rendered part.
types::page::idx_t get_page_under_edit(State::root_t state);

/// @brief Select current step sequence panel page size
unsigned int get_steps_page_size(std::shared_ptr<IO>, std::shared_ptr<GridLayouts>);

#endif
