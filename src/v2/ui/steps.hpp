#ifndef UI_STEPS_H
#define UI_STEPS_H

#include "anemone/ui/component.hpp"

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/state/instruments/names.hpp"
#include "anemone/types/sequencer.hpp"

#include <spdlog/spdlog.h>


namespace ui {

  class rendered_step_cursor_filter : public rx::Filter<std::shared_ptr<State::step_cursors_t>, State::step_cursor_t> {
  public:
    rendered_step_cursor_filter(std::shared_ptr<State::Root> state_wrapper) : state_wrapper(state_wrapper) {} ;
    virtual State::step_cursor_t filter(std::shared_ptr<State::step_cursors_t> cs) override {
      auto state = state_wrapper->state->get();
      auto instrument = state.instruments.rendered;
      auto part = state.instruments.by_name->at(instrument)->status.part.under_edit;

      return cs->at(instrument).at(part);
    };

    virtual bool predicate(State::step_cursor_t c) override {
      // todo make this a little more constrained...
      return true;
    };

  private:
    std::shared_ptr<State::Root> state_wrapper;
  };

  class rendered_steps_filter : public rx::Filter<std::shared_ptr<State::sequences_t>, types::rendered_steps_t> {
  public:
    rendered_steps_filter(std::shared_ptr<State::Root> state_wrapper) : state_wrapper(state_wrapper) {} ;
    virtual types::rendered_steps_t filter(std::shared_ptr<State::sequences_t> sqns) override {

      auto state = state_wrapper->state->get();
      auto instrument = state.instruments.rendered;
      auto part = state.instruments.by_name->at(instrument)->status.part.under_edit;

      spdlog::debug("CREATING A COPY OF A RENDERED_STEPS_T MAP");
      
      return sqns->at(instrument).at(part).rendered_steps;
    };

    virtual bool predicate(types::rendered_steps_t c) override {
      // todo make this a little more constrained...
      return true;
    };

  private:
    std::shared_ptr<State::Root> state_wrapper;
  };
  
  struct instrument_part_state {
    State::InstrumentName instrument;
    types::part::idx_t    part;
    types::page::idx_t    rendered_page;
    
    // TODO rendered steps
    // TODO track show_last_step;
    
    bool operator==(const instrument_part_state& rhs) {
      return
        instrument    == rhs.instrument &&
        part          == rhs.part       &&
        rendered_page == rhs.rendered_page;
    };
  };
  
  class Steps : public Component {
  public:
    std::shared_ptr<Diff<instrument_part_state> > steps;
    std::shared_ptr<Diff<types::step::paged_idx_t> > cursor;
    std::shared_ptr<Diff<types::rendered_steps_t> > rendered_steps;

    std::shared_ptr<rendered_step_cursor_filter> step_cursor_filter;
    std::shared_ptr<rendered_steps_filter> steps_rendered_filter;
    
    Steps(std::shared_ptr<State::Root>, std::shared_ptr<Grid>);

    virtual void connect() override;
    virtual void render() override;

  private:
    void render_cursor();
    void render_active_steps();
  };
  
}

#endif
