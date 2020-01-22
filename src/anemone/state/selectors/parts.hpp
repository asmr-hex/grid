/**
 * @file   state/selectors/parts.hpp
 * @brief  State Selectors For Parts
 * @author coco
 * @date   2020-01-19
 *************************************************/


#ifndef STATE_SELECTORS_PARTS_H
#define STATE_SELECTORS_PARTS_H

#include "anemone/state/root.hpp"
#include "anemone/types.hpp"


/// @brief Select the part which is currently rendered.
State::part_t get_rendered_part(State::root_t state);

/// @brief Select the id of the part under edit (rendered).
types::part::idx_t get_rendered_part_id(State::root_t state);

#endif
