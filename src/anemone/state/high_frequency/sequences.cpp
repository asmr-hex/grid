#include "anemone/state/high_frequency/sequences.hpp"

#include "anemone/util/match.hpp"


State::Sequences::Sequences() {

  int n_parts = 49; // TODO maybe read in a config?
  std::map<InstrumentName, std::map<unsigned int, sequence_t> > sequences;
  for (int i = 0; i < n_parts; i++) {
    sequences[InstrumentName::ER1][i] = sequence_t{};
  }
  
  state = rx::HighFrequency::State<sequences_t>
    ::with_reducer<high_freq_action_t>
    (sequences,
     [] (std::shared_ptr<sequences_t> sequences, high_freq_action_t action) {
       return
         match(action,
               [] (const auto& a) {
               });
     },
     [] (high_freq_action_t action) -> bool {
       return
         match(action,
               [] (const auto& a) {
                 return true;
               });
     });
}

std::shared_ptr<rx::dag::Observable<std::shared_ptr<State::sequences_t> > > State::Sequences::get() {
  return state;
}

