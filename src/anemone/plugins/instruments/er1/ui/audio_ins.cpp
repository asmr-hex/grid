#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/ui/audio_ins.hpp"


ER1::AudioInsUI::AudioInsUI(LayoutName layout_name,
                            GridSectionName section,
                            std::shared_ptr<IO> io,
                            std::shared_ptr<State> state,
                            std::shared_ptr<ER1> er1)
  : UIComponent(layout_name, section, io, state)
{
  for (auto i = 0; i < er1->pads.audio_ins.size(); i++) {
    auto audio_in = er1->pads.audio_ins[i];

    audio_in->is_playing
      .get_observable()
      .subscribe([this, i] (bool on) {
                   if (on) { turn_on_led(i); }
                   else    { turn_off_led(i); }                   
                 });
  }

}
