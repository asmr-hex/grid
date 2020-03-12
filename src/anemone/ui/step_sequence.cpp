#include <spdlog/spdlog.h>

#include "anemone/ui/step_sequence.hpp"


StepSequenceUI::StepSequenceUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();

  auto rendered_page = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.rendered.get_observable();
              })
    | rx::switch_on_next();

  auto added_steps = rendered_page
    | rx::map([this, state] (page_idx_t rendered_page) {
                auto rendered_instrument = state->instruments->rendered.get_value();
                auto rendered_part = rendered_instrument->status.part.under_edit.get_value();

                // since this is a new page, lets clear the page
                clear();

                try {
                  std::vector<grid_section_index> rendered_steps_vector;
                  auto rendered_steps_set = rendered_part->sequence.rendered_steps.at(rendered_page);
                  for (auto rendered_step : rendered_steps_set) {
                    rendered_steps_vector.push_back(rendered_step);
                  }

                  // set internal rendered steps
                  rendered_steps = rendered_steps_set;
                  
                  // now lets render all steps for this page.
                  turn_on_leds(rendered_steps_vector);
                } catch (std::out_of_range &error) {
                  // there are no rendered steps on this page....carry on.
                  rendered_steps = {};
                }

                // return the stream of added rendered steps only for this page.
                return rendered_part->sequence.added_steps.get_observable()
                  | rx::filter([rendered_page] (paged_step_idx_t paged_step) {
                                 return paged_step.page == rendered_page;
                               })
                  | rx::map([] (paged_step_idx_t paged_step) {
                              return paged_step.step;
                            });
              })
    | rx::switch_on_next();
 
  auto current_step = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.current.get_observable();
              })
    | rx::switch_on_next()  
    | rx::map([state] (granular_step_idx_t granular_step) {
                auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                return granular_to_paged_step(granular_step, page_size);
              });

  added_steps
    .subscribe([this] (page_relative_step_idx_t step) {
                 rendered_steps.insert(step);

                 turn_on_led(step);
               });
  
  rendered_page.combine_latest(current_step)
    .subscribe([this, state] (std::tuple<page_idx_t, paged_step_idx_t> p) {
                 auto page_size = state->layout->get_layouts()->sequencer->steps->size();
                 auto page = std::get<0>(p);
                 auto paged_step = std::get<1>(p);

                 if (page == paged_step.page) {
                   // turn on this step
                   turn_on_led(paged_step.step);

                   if (paged_step.step != 0 && rendered_steps.find(paged_step.step - 1) == rendered_steps.end()) {
                     turn_off_led(paged_step.step - 1);
                   }
                 }

                 auto was_final_step_on_page = ((page + 1) == paged_step.page || paged_step.page == 0 ) && paged_step.step == 0;
                 auto final_step_on_page_activated = rendered_steps.find(page_size - 1) == rendered_steps.end();
                 
                 if (was_final_step_on_page && !final_step_on_page_activated) {
                   // turn off last step on this page if cursor moved to a new page.
                   turn_off_led(page_size - 1);
                 }
               });
  
  // subscribe to the currently rendered part's step cursor location.
}
