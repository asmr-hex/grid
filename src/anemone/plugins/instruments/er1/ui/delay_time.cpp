#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/ui/delay_time.hpp"


ER1::DelayTimeUI::DelayTimeUI(LayoutName layout_name,
                    GridSectionName section,
                    std::shared_ptr<IO> io,
                    std::shared_ptr<State> state,
                    std::shared_ptr<ER1> er1)
  : UIComponent(layout_name, section, io, state)
{

  er1->parameters[0]->value.current
    .get_observable()
    .subscribe([this] (unsigned int value) {
                 set_led(0, 15 * value / 127);
               });
}
