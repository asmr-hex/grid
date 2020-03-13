#include <spdlog/spdlog.h>

#include "anemone/ui/show_last_step.hpp"


ShowLastStepUI::ShowLastStepUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument){
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([this](std::shared_ptr<Part> rendered_part) {
                // side-effect only map.

                // if new part is rendered, clear the show last step ui
                clear();

                return rendered_part;
              });
  
  auto show_last_step = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.show_last.get_observable();
              })
    | rx::switch_on_next();

  auto follow_cursor = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.follow_cursor.get_observable();
              })
    | rx::switch_on_next();
  
  // show last step ui logic.
  show_last_step.combine_latest(follow_cursor)
    .subscribe([this] (std::tuple<bool, bool> t) {
                 auto show_last_step = std::get<0>(t);
                 auto follow_cursor  = std::get<1>(t);

                 if (follow_cursor) {
                   // if we are following cursor

                   if (show_last_step && !previous.show_last_step)
                     remove_animation(0, 0);

                   if (!show_last_step && (!previous.follow_cursor || previous.show_last_step))
                     add_animation(led_level.animate.follow_cursor, 0);
                   
                 } else {
                   // if we are not following cursor

                   if (!show_last_step && previous.show_last_step)
                     turn_off_led(0);

                   // turn off animation if we are no longer following cursor
                   if (previous.follow_cursor)
                     remove_animation(0, 0);
                 }

                 if (show_last_step && !previous.show_last_step)
                   set_led(0, led_level.show_last_step);
                 
                 // update previous values
                 previous.follow_cursor = follow_cursor;
                 previous.show_last_step = show_last_step;
               });
}
