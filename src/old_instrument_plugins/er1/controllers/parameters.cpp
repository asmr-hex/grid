#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/er1/controllers/parameters.hpp"


ER1::ParametersController::ParametersController(std::shared_ptr<IO> io,
                                                std::shared_ptr<State> state,
                                                std::shared_ptr<ER1> er1)
{
  // setup press logic HACK SPECIFICIALLY FOR DELAY TIME
    auto delay_time_press = io->grid_events
      | rx::filter([] (grid_event_t e) {
                     return
                       e.type    == GridEvent::Pressed         &&
                       e.layout  == "ER1"            &&
                       e.section == "DelayTime";
                   });
    auto delay_time_unpress = io->grid_events
      | rx::filter([] (grid_event_t e) {
                     return
                       e.type    == GridEvent::Unpressed       &&
                       e.layout  == "ER1"            &&
                       e.section == "DelayTime";
                   });
    delay_time_press
      .subscribe([er1] (grid_event_t e) {
                   er1->parameters[1]->midi_map.mapping_in_progress.get_subscriber().on_next(true);
                 });
    delay_time_unpress
      .subscribe([er1] (grid_event_t e) {
                   er1->parameters[1]->midi_map.mapping_in_progress.get_subscriber().on_next(false);
                 });
    
  // subscribe each global parameter to the midi input it is mapped to
  for (auto parameter : er1->parameters) {
    // setup midi handling logic
    auto parameter_events = io->midi_events
      | rx::filter([parameter] (midi_event_t e) {
                     // is this event mapped to this parameter?
                     return parameter->midi_map.mapping_in_progress.get_value() ? true : parameter->is_mapped_to(e);
                   });

    parameter_events
      .subscribe([parameter, io] (midi_event_t e) {
                   if (parameter->midi_map.mapping_in_progress.get_value()) {
                     parameter->midi_map.source = e.source;
                     parameter->midi_map.status_byte = (unsigned int)e.data[0];
                     parameter->midi_map.control = (unsigned int)e.data[1];
                   } else {
                     // update the current parameter value with this new value
                     parameter->set_current_value(e);

                     // emit value to output
                     io->midi->emit(parameter->get_midi_output());
                   }
                 });
  }
}
