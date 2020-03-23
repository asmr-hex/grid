#include "anemone/plugins/instruments/er1/ui/osc1.hpp"


ER1::OSC1_UI::OSC1_UI(LayoutName layout_name, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout_name, section, io, state)
{
  // state->controls->shift
  //   .get_observable()
  //   .subscribe([this] (bool on) {
  //                if (on) { turn_on_led(0); }
  //                else    { turn_off_led(0); }
  //              });
}
