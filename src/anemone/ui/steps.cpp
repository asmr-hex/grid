#include "anemone/ui/steps.hpp"


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(state, grid) {
  // do some stuff
  filter = std::make_shared<rendered_step_cursor_filter>(state);
  
}


void ui::Steps::connect() {
  subscribe<State::root_t, instrument_part_state>
    (state,
     steps,
     [] (State::root_t state) -> instrument_part_state {
       auto instrument = state.instruments.rendered;
       auto part = state.instruments.by_name->at(instrument)->status.part.under_edit;
       auto rendered_page = state.instruments.by_name->at(instrument)->parts->at(part).page.rendered;
       return {
               .instrument    = instrument,
               .part          = part,
               .rendered_page = rendered_page,
       };
     });

  subscribe<std::shared_ptr<State::step_cursors_t>, State::step_cursor_t, types::step::page_relative_idx_t>
    (state->step_cursors,
     cursor,
     [] (State::step_cursor_t state) -> types::step::page_relative_idx_t {
       return state.current_page_relative_step;
     },
     filter);

  // todo filter for currently rendered instrument/part
}

void ui::Steps::render() {
  
}
