#include "anemone/ui/shift.hpp"


ShiftUI::ShiftUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  state->controls->shift
    .get_observable()
    .subscribe([this] (bool on) {
                 if (on) { turn_on_led(0); }
                 else    { turn_off_led(0); }
               });
}
