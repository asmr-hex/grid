#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/microgranny/microgranny.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/controller.hpp"


MicroGranny::Controller::Controller(std::shared_ptr<IO> io,
                            std::shared_ptr<State> state,
                            std::shared_ptr<MicroGranny> microgranny_state)
{

  midi = std::make_unique<MidiController>(io, state, microgranny_state);
  pads = std::make_unique<PadsController>(io, state, microgranny_state);
  
  // auto preset_select_events = io->grid_events
  //   | rx::filter([] (grid_event_t e) {
  //                  return
  //                    e.layout  == LayoutName::MicroGranny             &&
  //                    e.section == GridSectionName::MicroGrannyPresets &&
  //                    e.type    == GridEvent::Pressed;
  //                });

  // preset_select_events
  //   .subscribe([io, state, microgranny_state] (grid_event_t e) {
                                   
  //                midi_event_t midi_event = { .source      = "",
  //                                            .destination = "",
  //                                            .data        = make_cc_message(microgranny_state->midi_map.channel,
  //                                                                           0,
  //                                                                           e.index * 10),
  //                };

  //                spdlog::warn("INDEX: {}", e.index);
                 
  //                // emit preset midi cc
  //                io->midi->emit(midi_event);
  //              });


  // auto on_midi_notes = io->midi_events
  //   | rx::filter([] (midi_event_t e) {
  //                  return (unsigned int)e.data[0] >= 144 && (unsigned int)e.data[0] <= 159;
  //                });

  // on_midi_notes
  //   .subscribe([io, microgranny_state] (midi_event_t e) {
  //                spdlog::warn("MIDI NOTE");
                 
  //                // convert incoming to microgranny channel
  //                e.data[0] = (unsigned char)(143 + microgranny_state->midi_map.channel);

  //                io->midi->emit(e);
  //              });


  // auto off_midi_notes = io->midi_events
  //   | rx::filter([] (midi_event_t e) {
  //                  return (unsigned int)e.data[0] >= 128 && (unsigned int)e.data[0] <= 143;
  //                });

  // off_midi_notes
  //   .subscribe([io, microgranny_state] (midi_event_t e) {
  //                spdlog::warn("MIDI NOTE");
                 
  //                // convert incoming to microgranny channel
  //                e.data[0] = (unsigned char)(127 + microgranny_state->midi_map.channel);

  //                io->midi->emit(e);
  //              });


  
  // auto off_events = io->grid_events
  //   | rx::filter([] (grid_event_t e) {
  //                  return
  //                    e.layout  == LayoutName::MicroGranny       &&
  //                    e.section == GridSectionName::OSC1 &&
  //                    e.type    == GridEvent::Unpressed;
  //                });

  // off_events.subscribe([io, state, microgranny_state] (grid_event_t e) {
  //                       // emit ocs1 midi on note
  //                       io->midi->emit({ .source      = "",
  //                                        .destination = "",
  //                                        .data        = midi_note_off(microgranny_state->midi_map.notes.osc1,
  //                                                                    microgranny_state->midi_map.channel),
  //                         });

  //                       microgranny_state->pad_is_playing.osc1.get_subscriber().on_next(false);
  //                     });

  // auto playback_midi_note_on_events = microgranny_state->playback_midi_events.get_observable()
  //   | rx::filter([microgranny_state] (midi_event_t midi_event) {
  //                  if (midi_event.data.size() == 0) return false;
                   
  //                  midi_data_t osc1_midi_on = midi_note_on(microgranny_state->midi_map.notes.osc1,
  //                                                          microgranny_state->midi_map.channel,
  //                                                          127);
                   
  //                  // filter only for notes corresponding to osc1 not on
  //                  return
  //                    midi_event.data[0] == osc1_midi_on[0] &&
  //                    midi_event.data[1] == osc1_midi_on[1];

  //                });
  // playback_midi_note_on_events
  //   .subscribe([microgranny_state] (midi_event_t midi_event) {
  //                microgranny_state->pad_is_playing.osc1.get_subscriber().on_next(true);
  //              });
    
  // auto playback_midi_note_off_events = microgranny_state->playback_midi_events.get_observable()
  //   | rx::filter([microgranny_state] (midi_event_t midi_event) {
  //                  if (midi_event.data.size() == 0) return false;

  //                  midi_data_t osc1_midi_on = midi_note_off(microgranny_state->midi_map.notes.osc1,
  //                                                           microgranny_state->midi_map.channel);
                   
  //                  // filter only for notes corresponding to osc1 not on
  //                  return
  //                    midi_event.data[0] == osc1_midi_on[0] &&
  //                    midi_event.data[1] == osc1_midi_on[1];
  //                });
  // playback_midi_note_off_events
  //   .subscribe([er1_state] (midi_event_t midi_event) {
  //                er1_state->pad_is_playing.osc1.get_subscriber().on_next(false);
  //              });

  // // nrpn test.
  // auto knobs_cc_events = io->midi_events
  //   | rx::filter([] (midi_event_t midi_event) {
  //                  return
  //                    midi_event.source                  == "Midi Fighter Twister" &&
  //                    ((unsigned int)midi_event.data[0]) == 176 &&  // cc + channel 1
  //                    ((unsigned int)midi_event.data[1]) == 16; // control 16
  //                });

  // knobs_cc_events
  //   .subscribe([io] (midi_event_t midi_event) {
  //                auto value = (unsigned int)midi_event.data[2];

  //                // output nrpn message to control er1 delay
  //                midi_data_t nrpn_data_lsb;
  //                nrpn_data_lsb.push_back((unsigned char)(175 + 10)); // set channel + cc status
  //                nrpn_data_lsb.push_back((unsigned char)(98));       // 98 is the controller to specify nrpn LSB
  //                nrpn_data_lsb.push_back((unsigned char)(101));        // NRPN parameter select LSB

  //                midi_data_t nrpn_data_msb;
  //                nrpn_data_msb.push_back((unsigned char)(175 + 10)); // set channel + cc status
  //                nrpn_data_msb.push_back((unsigned char)(99));       // 98 is the controller to specify nrpn MSB
  //                nrpn_data_msb.push_back((unsigned char)(2));        // NRPN parameter select MSB

  //                midi_data_t nrpn_data_entry;
  //                nrpn_data_entry.push_back((unsigned char)(175 + 10)); // set channel + cc status
  //                nrpn_data_entry.push_back((unsigned char)(6));        // 6 is the controller to specify nrpn Data entry
  //                nrpn_data_entry.push_back((unsigned char)(value));        // NRPN parameter set
                 
  //                io->midi->emit({ .source = "", .destination = "", .data = nrpn_data_lsb });
  //                io->midi->emit({ .source = "", .destination = "", .data = nrpn_data_msb });
  //                io->midi->emit({ .source = "", .destination = "", .data = nrpn_data_entry });

  //              });
  
}
