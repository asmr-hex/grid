/**
 * @file   types/isntrument/sequence.hpp
 * @brief  Sequence related types.
 * @author coco
 * @date   2020-02-20
 *************************************************/


#ifndef ANEMONE_TYPES_INSTRUMENT_SEQUENCE_H
#define ANEMONE_TYPES_INSTRUMENT_SEQUENCE_H

#include <map>
#include <set>

#include "anemone/types/instrument/part.hpp"
#include "anemone/types/instrument/page.hpp"
#include "anemone/types/instrument/step.hpp"
#include "anemone/types/instrument/step_event.hpp"


/// @brief a sequence layer type.
///
/// @details
/// The sequence layer data structure stores sequencer step events in a map keyed
/// by a unique event id modulo certain characteristic of the event. In other words,
/// each layer is a step event which can be distinguished from other step events in
/// a particular way. Currently, the unique id is computed from the midi channel from
/// which the event came from, the "type" of midi event (e.g. cc, note, nrpn, etc...),
/// and the "value" of the event (e.g. the cc, note, or nrpn value, etc...). Structuring
/// the data this way allows for (1) polyphony (2) efficiently muting/soloing specific
/// voices.
///
typedef std::map<step_event_id_t, step_event_t> sequence_layer_t;


/// @brief type alias for sequence storing data structure.
typedef std::map<step_idx_t, sequence_layer_t> sequence_t;

/// @brief data structure for storing rendered steps
///
/// @todo eventually this must be able to handle layering!
typedef std::map<page_idx_t, std::set<page_relative_step_idx_t> > rendered_steps_t;

#endif
