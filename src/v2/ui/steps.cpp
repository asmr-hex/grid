#include "anemone/ui/steps.hpp"

#include <algorithm>

#include <spdlog/spdlog.h>


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(LayoutName::SequencerAndInstrument, GridSectionName::Steps, state, grid) {

  // initialize ui states
  steps = std::make_shared<Diff<instrument_part_state> >(instrument_part_state{});
  cursor = std::make_shared<Diff<types::step::paged_idx_t> >(types::step::paged_idx_t{});
  rendered_steps = std::make_shared<Diff<types::rendered_steps_t> >(types::rendered_steps_t{});

  // initialize observable subscription filter
  step_cursor_filter = std::make_shared<rendered_step_cursor_filter>(state);
  steps_rendered_filter = std::make_shared<rendered_steps_filter>(state);
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
            types::step::paged_idx_t>
    (state->step_cursors,
     cursor,
     [] (State::step_cursor_t state) -> types::step::paged_idx_t {
       return state.current_page_relative_step;
     },
     step_cursor_filter);

  
  subscribe<std::shared_ptr<State::sequences_t>,
            types::rendered_steps_t,
            types::rendered_steps_t>
    (state->sequences,
     rendered_steps,
     [] (types::rendered_steps_t state) -> types::rendered_steps_t {
       return state;
     },
     steps_rendered_filter);
}

void ui::Steps::render() {
  // TODO implement me!
  // spdlog::debug("rendering ui steps");

  render_cursor();

  render_active_steps();
}

void ui::Steps::render_active_steps() {
  if ( rendered_steps->previous == rendered_steps->current )
    return;

  spdlog::debug("RE-REDNERING RENDERED STEPS");

  // previous and current rendered pages
  auto previous = rendered_steps->previous[steps->previous.rendered_page];
  auto current = rendered_steps->current[steps->current.rendered_page];
  
  // get steps to turn off
  std::set<types::step::page_relative_idx_t> off_steps;
  std::set_difference(previous.begin(), previous.end(),
                      current.begin(), current.end(),
                      std::inserter(off_steps, off_steps.begin()));
  for (auto step : off_steps) {
    turn_off_led(step);
  }
  
  // get steps to turn on
  std::set<types::step::page_relative_idx_t> on_steps;
  std::set_difference(current.begin(), current.end(),
                      previous.begin(), previous.end(),
                      std::inserter(on_steps, on_steps.begin()));
  for (auto step : on_steps) {
    turn_on_led(step);
  }
}

void ui::Steps::render_cursor() {
  // if cursor hasn't changed, do nothing
  if ( cursor->previous == cursor->current )
    return;

  // TODO DONT TURN OFF STEPS IF THEY ARE ACTIVATED
  auto previous_rendered_steps = rendered_steps->previous[steps->previous.rendered_page];
  // auto current_rendered_steps = rendered_steps->current[steps->current.rendered_page];
  // TODO make helper function!
  
  // turn off last step
  if ( steps->previous.rendered_page == cursor->previous.page &&
       previous_rendered_steps.find(cursor->previous.step) == previous_rendered_steps.end())
    turn_off_led(cursor->previous.step);

  // turn on next step
  if ( steps->current.rendered_page == cursor->current.page )
    turn_on_led(cursor->current.step);
}
