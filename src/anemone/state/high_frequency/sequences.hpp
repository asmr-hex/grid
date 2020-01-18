/**
 * @file   state/high_frequency/sequences.hpp
 * @brief  Sequences State (High Frequency)
 * @author coco
 * @date   2020-01-17
 *************************************************/


#ifndef STATE_HIGH_FREQUENCY_SEQUENCES_H
#define STATE_HIGH_FREQUENCY_SEQUENCES_H

#include <map>
#include <memory>

#include "anemone/rx/state.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/action/types.hpp"

#include "anemone/types.hpp"


namespace State {

  /// @brief state sequence type containing various types of midi sequences.
  struct sequence_t {
    types::sequence_t midi_on;
    types::sequence_t midi_off;
    types::sequence_t midi_cc;
    types::sequence_t midi_nrpn;
  };

  /// @brief mapping of instrument to part to sequences.
  typedef std::map<InstrumentName, std::map<unsigned int, sequence_t> > sequences_t;

  /// @brief ...
  class Sequences : public rx::Observable<std::shared_ptr<sequences_t> > {
  public:
    rx::types::high_frequency::state_ptr<sequences_t, high_freq_action_t> state;

    Sequences();

    virtual std::shared_ptr<rx::dag::Observable<std::shared_ptr<sequences_t> > > get() override;
  };
  
}

#endif
