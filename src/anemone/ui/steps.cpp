#include "anemone/ui/steps.hpp"

#include <spdlog/spdlog.h>


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(LayoutName::SequencerAndInstrument, GridSectionName::Steps, state, grid) {

  // initialize ui states
  steps = std::make_shared<Diff<instrument_part_state> >(instrument_part_state{});
  cursor = std::make_shared<Diff<types::step::page_relative_idx_t> >(types::step::page_relative_idx_t{});

  // initialize observable subscription filter
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

  subscribe<std::shared_ptr<State::step_cursors_t>,
            State::step_cursor_t,
            types::step::page_relative_idx_t>
    (state->step_cursors,
     cursor,
     [] (State::step_cursor_t state) -> types::step::page_relative_idx_t {
       return state.current_page_relative_step;
     },
     filter);
}

void ui::Steps::render() {
  // TODO implement me!
  spdlog::debug("rendering ui steps");

  render_cursor();
}

void ui::Steps::render_cursor() {
  // if cursor hasn't changed, do nothing
  if ( cursor->previous == cursor->current )
    return;
  
  // turn off last step
  if ( steps->previous.rendered_page == cursor->previous.page )
    turn_off_led(cursor->previous.step);

  // turn on next step
  if ( steps->current.rendered_page == cursor->current.page )
    turn_on_led(cursor->current.step);
}
