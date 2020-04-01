#include "anemone/ui/instrument_select.hpp"


InstrumentSelectUI::InstrumentSelectUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_instrument_index = state->instruments->rendered.get_observable()
    | rx::map([this, state] (std::shared_ptr<Instrument> rendered_instrument) -> unsigned int {
                for (auto itr : state->instruments->by_index) {
                  clear();
                  if (rendered_instrument->name == itr.second->name) return itr.first;
                }
                // should never get here, but...
                return 0;
              });
    
  rendered_instrument_index
    .subscribe([this] (unsigned int rendered_instrument_index) {
                 set_led(rendered_instrument_index, 10);
               });
}
