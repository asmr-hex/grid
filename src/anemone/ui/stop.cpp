#include <spdlog/spdlog.h>

#include "anemone/ui/stop.hpp"


StopUI::StopUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part_is_stopping_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> part) {
                return part->transport.is_stopping.get_observable();
              })
    | rx::switch_on_next();

  // page ui logic.
  rendered_part_is_stopping_observable
    .subscribe([this] (bool part_is_stopping) {

                 if (part_is_stopping) {
                   turn_on_led(0);
                 } else {
                   turn_off_led(0);
                 }

               });
}
