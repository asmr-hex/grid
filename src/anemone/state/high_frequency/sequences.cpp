#include "anemone/state/high_frequency/sequences.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


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
               [sequences] (const action::step_updated& a) {
                 if (a.activated) {
                   // remove from midi on events
                   sequences->at(a.instrument_name)[a.part].midi_on[a.step].erase(a.event.id);

                   // add rendered steps
                   sequences->at(a.instrument_name)[a.part].rendered_steps[a.paged_step.page].erase(a.paged_step.step);
                 } else {
                   // insert into midi on events
                   sequences->at(a.instrument_name)[a.part].midi_on[a.step][a.event.id] = a.event;

                   // TODO figure out how to insert midi off event

                   // add rendered steps
                   sequences->at(a.instrument_name)[a.part].rendered_steps[a.paged_step.page].insert(a.paged_step.step);

                   // spdlog::debug("rendered steps: {}", sequences->at(a.instrument_name)[a.part].rendered_steps[a.paged_step.page].size()); 
                 }
               },
               [] (const auto& a) {
               });
     },
     [] (high_freq_action_t action) -> bool {
       return
         match(action,
               [] (const action::step_updated& a) {
                 // important! otherwise it will broadcast EVERY time another high frequency
                 // action occurs! AKA a clock tick!
                 return true;
               },
               [] (const auto& a) {
                 return false;
               });
     });
}

std::shared_ptr<rx::dag::Observable<std::shared_ptr<State::sequences_t> > > State::Sequences::get() {
  return state;
}

