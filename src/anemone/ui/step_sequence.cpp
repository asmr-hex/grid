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
                 auto rendered_page = std::get<0>(p);
                 auto current_paged_step = std::get<1>(p);

                 
                 // when the rendered page is the same as the playing page
                 if (rendered_page == current_paged_step.page) {
                   auto current_step_active =
                     rendered_page == current_paged_step.page &&
                     rendered_steps.find(current_paged_step.step) != rendered_steps.end();
                   auto previous_step_active =
                     rendered_steps.find(current_paged_step.step - 1) != rendered_steps.end();

                   // turn on this step
                   set_led(current_paged_step.step,
                           current_step_active ? led_level.cursor_on_active_step : led_level.cursor);

                   // when the current step is *not* the first step on the page
                   // and the previous step is *not* an active (on) step, turn off the previous step.
                   if (current_paged_step.step != 0) {
                     set_led(current_paged_step.step - 1,
                             previous_step_active ? led_level.active : led_level.inactive);
                   }
                 } else {
                   // when the rendered page is *not* the same as the playing page

                   // was the previous step the last step on the currently rendered page?
                   auto was_final_step_on_page =
                     ((rendered_page + 1) == current_paged_step.page || current_paged_step.page == 0 )
                     && current_paged_step.step == 0;

                   // is the final step on this page activated (on)?
                   auto final_step_on_page_activated = rendered_steps.find(page_size - 1) != rendered_steps.end();

                   // if the previous step was the final step on the page AND the final step on the
                   // page was *not* activated, turn off the final step on this page.
                   if (was_final_step_on_page && !final_step_on_page_activated)
                     set_led(page_size - 1,
                             final_step_on_page_activated ? led_level.active : led_level.inactive);
                 }
               });
}
