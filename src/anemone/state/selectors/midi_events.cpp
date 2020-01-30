#include "anemone/state/selectors/midi_events.hpp"

#include "anemone/state/selectors/parts.hpp"
#include "anemone/state/selectors/step_cursor.hpp"

#include <spdlog/spdlog.h>


void get_midi_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results) {
  get_midi_on_events_for(instrument, state, results);
}

void get_midi_on_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t>* results) {
  // we want to get the currently playing part of this instrument
  auto part = get_playing_part_id(instrument);

  // we want to get the current step for this part
  auto step_cursor = get_step_cursor_for(instrument, part, state);

  // get the sequence layers at this step
  // TODO i've hard coded the page size, but WE SHOULD INTRODUCE THIS INTO THE STATE SOMEHOW!
  // auto layers = &state.sequences->at(instrument.name)[part].midi_on[step_cursor.current_page_relative_step.step];
  auto layers = &state.sequences->at(instrument.name)[part].midi_on[step_cursor.current_page_relative_step.to_absolute_idx(32)];

  // put all layers into one vector
  for (auto itr : *layers) {
    results->push_back(itr.second);
  }
}

void get_midi_off_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results) {
  
}

void get_midi_cc_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results) {
  
}

void get_midi_nrpn_events_for(State::instrument_t instrument, State::root_t state, std::vector<types::step::event_t> *results) {
  
}
