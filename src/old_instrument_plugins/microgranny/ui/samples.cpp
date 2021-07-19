#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/microgranny/microgranny.hpp"
#include "anemone/plugins/instruments/microgranny/ui/samples.hpp"


MicroGranny::SamplesUI::SamplesUI(LayoutName layout_name,
                                  GridSectionName section,
                                  std::shared_ptr<IO> io,
                                  std::shared_ptr<State> state,
                                  std::shared_ptr<MicroGranny> microgranny)
  : UIComponent(layout_name, section, io, state)
{

  
  // subscribe to ui changes for all sample pads
  for (auto i = 0; i < microgranny->pad_is_playing.size(); i++) {
    microgranny->pad_is_playing[i]
      .get_observable()
      .subscribe([this, i] (bool on) {
                   if (on) { turn_on_led(i); }
                   else    { turn_off_led(i); }    
                 });      
  }
}
