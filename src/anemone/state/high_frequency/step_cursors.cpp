#include "anemone/state/high_frequency/step_cursors.hpp"

#include "anemone/util/match.hpp"


State::StepCursors::StepCursors() {
  // initialize empty map of steps

  
  state = rx::HighFrequency::State<step_cursors_t>
    ::with_reducer<high_freq_action_t>
    ({},
     [] (std::shared_ptr<step_cursors_t> steps, high_freq_action_t action) {
       return match(action,
                    [] (const auto& a) {
                    });
     },
     [] (high_freq_action_t action) -> bool {
       return match(action,
                    [] (const auto& a) {
                      return false;
                    });
     });
}

std::shared_ptr<rx::dag::Observable<std::shared_ptr<State::step_cursors_t> > > State::StepCursors::get() {
  return state;
}
