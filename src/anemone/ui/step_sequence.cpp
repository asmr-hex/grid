#include <spdlog/spdlog.h>

#include "anemone/ui/step_sequence.hpp"


StepSequenceUI::StepSequenceUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();

  auto rendered_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.rendered.get_observable();
              })
    | rx::switch_on_next();

  auto current_step_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.current.get_observable();
              })
    | rx::switch_on_next()  
    | rx::map([state] (granular_step_idx_t granular_step) {
                auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                return granular_to_paged_step(granular_step, page_size);
              });
  
  rendered_page_observable.combine_latest(current_step_observable)
    .subscribe([this, state] (std::tuple<page_idx_t, paged_step_idx_t> p) {
                 auto page_size = state->layout->get_layouts()->sequencer->steps->size();
                 auto page = std::get<0>(p);
                 auto paged_step = std::get<1>(p);

                 if (page == paged_step.page) {
                   // turn on this step
                   turn_on_led(paged_step.step);

                   if (paged_step.step != 0) {
                     turn_off_led(paged_step.step - 1);
                   }
                 }

                 if (page == (paged_step.page +1) && paged_step.step == 0) {
                   // turn off last step on this page if cursor moved to a new page.
                   turn_off_led(page_size-1);
                 }
               });
  
  // TODO do the same for steps and rendered steps --^

  // subscribe to the currently rendered part's step cursor location.
}
