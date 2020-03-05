/**
 * @file   state/instruments/instrument.hpp
 * @brief  Instrument State Base Class
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef STATE_INSTRUMENTS_INSTRUMENT_H
#define STATE_INSTRUMENTS_INSTRUMENT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"
#include "anemone/config.hpp"

#include "anemone/state/instruments/part.hpp"


class Instrument {
public:
  Instrument(InstrumentName,
             std::shared_ptr<Config>,
             std::vector<std::shared_ptr<Part> >,
             sequence_layer_t);

  InstrumentName name;

  struct status_t {
    struct part_t {
      rx::behavior<std::shared_ptr<Part> > in_playback;
      rx::behavior<std::shared_ptr<Part> > under_edit;
    };
    struct bank_t {
      rx::behavior<bank_idx_t> in_playback;
      rx::behavior<bank_idx_t> under_edit;
    };

    part_t part;
    bank_t bank;
    rx::behavior<bool> is_playing;
    rx::behavior<bool> stop_on_next_measure;
  };

  status_t status;
  std::vector<std::shared_ptr<Part> > parts;

  /// @brief the last midi notes played.
  ///
  /// @description this is used as the default note for when new steps are added by hand.
  rx::behavior<sequence_layer_t> last_midi_notes_played;
  
};

/// @brief wrapper over Instrument constructor to initialize parts.
Instrument create_instrument(InstrumentName, std::shared_ptr<Config>);

#endif
