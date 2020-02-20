/**
 * @file   state/instruments/sequence.hpp
 * @brief  Instrument Sequence Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_SEQUENCE_H
#define STATE_INSTRUMENTS_SEQUENCE_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"


class Sequence {
public:
  sequence_t       midi_on;
  sequence_t       midi_off;
  sequence_t       midi_cc;
  sequence_t       midi_nrpn;
  rendered_steps_t rendered_steps;
};

#endif
