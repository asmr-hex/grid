#include <spdlog/spdlog.h>

#include "anemone/ui/show_last_step.hpp"


ShowLastStepUI::ShowLastStepUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto show_last_step = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument){
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([this] (std::shared_ptr<Part> rendered_part) {
                // clear ui on new part
                clear();

                return rendered_part->step.show_last.get_observable();
              })
    | rx::switch_on_next();
  
  
  // show last step ui logic.
  show_last_step
    .subscribe([this] (bool show_last_step) {
                 if (show_last_step) { turn_on_led(0); }
                 else { turn_off_led(0); }

                 // TODO add logic for follow step!
               });
}
