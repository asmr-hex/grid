#include <spdlog/spdlog.h>

#include "anemone/ui/ppqn.hpp"


PPQNUI::PPQNUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state),
    ppqn_to_index({ { PPQN::One, 0},
                    { PPQN::Two, 1},
                    { PPQN::Four, 2},
                    { PPQN::Eight, 3},
                    { PPQN::Sixteen, 4},
                    { PPQN::ThirtyTwo, 5},
                    { PPQN::SixtyFour, 6},
      })
{
  auto current_ppqn_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->ppqn.current.get_observable();
              })
    | rx::switch_on_next();

  // ppqn ui logic.
  current_ppqn_observable
    .subscribe([this] (PPQN ppqn) {
                 turn_on_led(ppqn_to_index[ppqn]);
               });
}
