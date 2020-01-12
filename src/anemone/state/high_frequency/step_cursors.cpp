#include "anemone/state/high_frequency/step_cursors.hpp"

#include "anemone/util/match.hpp"

#include <spdlog/spdlog.h>


State::StepCursors::StepCursors() {

  int n_parts = 49; // TODO maybe read in a config?
  std::map<InstrumentName, std::map<unsigned int, step_cursor_t> > step_cursors;
  for (int i = 0; i < n_parts; i++) {
    step_cursors[InstrumentName::ER1][i] = step_cursor_t{};
  }

  state = rx::HighFrequency::State<step_cursors_t>
    ::with_reducer<high_freq_action_t>
    (step_cursors,
     [] (std::shared_ptr<step_cursors_t> steps, high_freq_action_t action) {
       return match(action,
                    [steps] (const action::step_updated& a) {
                      (*steps)[a.instrument_name][a.part].current_step = a.step;
                      (*steps)[a.instrument_name][a.part].current_page = a.page;
                    },
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
