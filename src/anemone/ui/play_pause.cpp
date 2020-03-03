#include <spdlog/spdlog.h>

#include "anemone/ui/play_pause.hpp"


PlayPauseUI::PlayPauseUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part_is_playing_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> part) {
                return part->transport.is_playing.get_observable();
              })
    | rx::switch_on_next();

  auto rendered_part_is_paused_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([] (std::shared_ptr<Part> part) {
                return part->transport.is_paused.get_observable();
              })
    | rx::switch_on_next();

  
  // page ui logic.
  rendered_part_is_playing_observable.combine_latest(rendered_part_is_paused_observable)
    .subscribe([this] (std::tuple<bool, bool> t) {
                 auto rendered_part_is_playing = std::get<0>(t);
                 auto rendered_part_is_paused  = std::get<1>(t);

                 if (rendered_part_is_playing) {
                   turn_on_led(0);
                 } else if (rendered_part_is_paused) {
                   set_led(0, 7);
                 } else {
                   turn_off_led(0);
                 }

               });
}
