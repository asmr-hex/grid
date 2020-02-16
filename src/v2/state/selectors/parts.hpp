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

/// @brief Select the id of the part in playback for the provided instrument.
///
/// @remark
/// If the instrument is not currently in playback itself, then the selected
/// part will not be playing (stopped or paused), though it is still the part
/// which is in playback mode for this isntrument.
/// TODO we might have to take into account the BANK!
types::part::idx_t get_playing_part_id(State::instrument_t instrument);
  
#endif
