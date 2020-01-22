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


/// @brief Select page under edit in the currently rendered part.
types::page::idx_t get_page_under_edit(State::root_t state);

#endif
