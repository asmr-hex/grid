#include <spdlog/spdlog.h>

#include "anemone/ui/parts.hpp"


PartsUI::PartsUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->id;
              });

  auto in_playback_part_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.in_playback.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->id;
              });

  
  // page ui logic.
  rendered_part_observable.combine_latest(in_playback_part_observable)
    .subscribe([this] (std::tuple<part_idx_t, part_idx_t> t) {
                 auto rendered_part = std::get<0>(t);
                 auto playing_part  = std::get<1>(t);

                 turn_on_led(playing_part);
               });
}
