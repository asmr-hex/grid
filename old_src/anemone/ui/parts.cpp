#include <spdlog/spdlog.h>

#include "anemone/ui/parts.hpp"


PartsUI::PartsUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state),
    previous({ .rendered_part = 0, .playing_part = 0 })
{
  auto rendered_part = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();
  
  auto rendered_part_observable = rendered_part
    | rx::map([state] (std::shared_ptr<Part> p) {
                // get the part section relative index of this part
                auto part_size = state->layouts->sequencer->parts->size();
                return p->id % part_size;
              });

  auto in_playback_part_observable = rendered_part
    | rx::map([state] (std::shared_ptr<Part> p) {
                // get the part section relative index of this part
                auto part_size = state->layouts->sequencer->parts->size();
                return p->id % part_size;
              });

  
  // page ui logic.
  rendered_part_observable.combine_latest(in_playback_part_observable)
    .subscribe([this] (std::tuple<unsigned int, unsigned int> t) {
                 auto rendered_part = std::get<0>(t);
                 auto playing_part  = std::get<1>(t);

                 // turn off the previous parts if they are not the same as the current parts
                 if (previous.rendered_part != rendered_part)
                   turn_off_led(previous.rendered_part);
                 if (previous.playing_part != playing_part)
                   turn_off_led(previous.playing_part);

                 // always turn on playing part and rendered part
                 set_led(playing_part, 6);
                 turn_on_led(rendered_part);

                 // update previous parts
                 previous.rendered_part = rendered_part;
                 previous.playing_part  = playing_part;
               });
}
