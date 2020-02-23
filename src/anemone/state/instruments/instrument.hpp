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
  Instrument(InstrumentName, std::shared_ptr<Config>);

  InstrumentName name;

  struct status_t {
    struct part_t {
      rx::behavior<std::shared_ptr<Part> > in_playback;
      rx::behavior<std::shared_ptr<Part> > under_edit;
    };
    struct bank_t {
      bank_idx_t in_playback = 0;
      bank_idx_t under_edit = 0;
    };

    part_t part;
    bank_t bank;
    bool   is_playing = false;
    bool   stop_on_next_measure = false;
  };

  status_t status;
  std::vector<std::shared_ptr<Part> > parts;
};

#endif
