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
                if (rendered_page != previous.rendered_page)
                  clear();

                try {
                  // collect all rendered steps for this page into a vector
                  std::vector<grid_section_index_t> rendered_steps_vector;
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
                auto page_size = state->layouts->sequencer->steps->size();

                return granular_to_paged_step(granular_step, page_size);
              });

  auto last_step = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.last.get_observable();
              })
    | rx::switch_on_next();
  
  auto show_last_step = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.show_last.get_observable();
              })
    | rx::switch_on_next();

  // render newly added steps to this page
  added_steps
    .subscribe([this] (page_relative_step_idx_t step) {
                 rendered_steps.insert(step);

                 turn_on_led(step);
               });
  
  rendered_page.combine_latest(current_step, show_last_step, last_step)
    .subscribe([this, state] (std::tuple<page_idx_t, paged_step_idx_t, bool, paged_step_idx_t> p) {
                 auto rendered_page      = std::get<0>(p);
                 auto current_paged_step = std::get<1>(p);
                 auto show_last_step     = std::get<2>(p);
                 auto last_step          = std::get<3>(p);

                 if (rendered_page == current_paged_step.page) {
                   // when the rendered page is the same as the playing page (cursor is on this page).
                   // we want to:
                   // 1) turn on the step where the cursor is located
                   //      - set the led brightness appropriately if the cursor step is on an active step
                   // 2) turn off the previous cursor step if if is on this page
                   //      - set the previous step led brighntess appropriately if it was on an active step
                   
                   auto current_step_active =
                     rendered_page == current_paged_step.page &&
                     rendered_steps.find(current_paged_step.step) != rendered_steps.end();
                   auto previous_step_active =
                     rendered_steps.find(current_paged_step.step - 1) != rendered_steps.end();

                   // 1) turn the current cursor step on (to the appropriate led brightness)
                   set_led(current_paged_step.step,
                           current_step_active ? led_level.cursor_on_active_step : led_level.cursor);

                   // 2) turn off the previous cursor step (to the appropriate led brightness)
                   // when the current step is *not* the first step on the page
                   // and the previous step is *not* an active (on) step, turn off the previous step.
                   if (current_paged_step.step != 0) {
                     set_led(current_paged_step.step - 1,
                             previous_step_active ? led_level.active : led_level.inactive);
                   }
                 }

                 // turn off the last step on the rendered page if the cursor just moved to a new page.
                 auto cursor_moved_to_next_page =
                   (rendered_page != last_step.page && current_paged_step == paged_step_idx_t{ .page=rendered_page + 1, .step=0 }) ||
                   (rendered_page == last_step.page && current_paged_step == paged_step_idx_t{ .page=0, .step=0 } );
                 if (cursor_moved_to_next_page) {
                   // get the last step on the rendered page
                   auto last_step_on_rendered_page =
                     rendered_page == last_step.page ?
                     last_step.step : state->layouts->sequencer->steps->size() - 1;
                     
                   // is the final step on this page activated (on)?
                   auto final_step_on_page_activated = rendered_steps.find(last_step_on_rendered_page) != rendered_steps.end();

                   // if the previous step was the final step on the page, set the led to the appropraite brightness.
                   // spdlog::warn("WAS FINAL STEP ON PAGE"); TODO: this fires a bunch of times, we can optimize so it only fires once
                   set_led(last_step_on_rendered_page,
                           final_step_on_page_activated ? led_level.active : led_level.inactive);                     
                 }
                 
                 // if show_last_step has been activated, we want to add an animation for the last step.
                 // since we are observing both rendered_page AND show_last_step (via combine_latest), they
                 // will not change in lock-step, but one-by-one, meaninng that we will never have a situation
                 // in which the previous rendered_page != current rendered_page AND previous show_last_step !=
                 // current show_last_step.
                 if (rendered_page == last_step.page &&
                     show_last_step &&
                     (previous.rendered_page != rendered_page || previous.show_last_step != show_last_step))
                   add_animation(led_level.animate.last_step, last_step.step);

                 // turn off the previous last step if it has changed!
                 if (rendered_page == previous.last_step.page &&
                     last_step != previous.last_step)
                   remove_animation(previous.last_step.step, 0);
                   
                 // turn on new last step animation if it has changed!
                 if (rendered_page == last_step.page &&
                     last_step != previous.last_step)
                   add_animation(led_level.animate.last_step, last_step.step);
                 
                 // remove last step animation if show_last_step has changed
                 // set the led to the appropriate brightness if the last_step was active
                 if (rendered_page == last_step.page && !show_last_step && previous.show_last_step)
                   remove_animation(last_step.step, 0);
                   

                 // update previous values
                 previous.rendered_page = rendered_page;
                 previous.last_step = last_step;
                 previous.show_last_step = show_last_step;
               });
}
