#include "anemone/plugins/instruments/er1/er1.hpp"

ER1::ER1::Pad::Pad(midi_spn_t spn, midi_channel_t chan)
  : midi({ .channel       = chan,
           .spn           = spn,
           .note_number   = spn_to_num(spn),
           .note_on_data  = midi_note_on(spn, chan, 127),
           .note_off_data = midi_note_off(spn, chan),
    })
{}

ER1::ER1::ER1(std::shared_ptr<Config> config)
  : Plugin(config),
    Instrument(create_instrument(InstrumentName::ER1, config, std::make_shared<ER1Layout>(ER1Layout(config))))
{
  // initialize midi map
  for (auto osc : pads.oscillators) {
    midi_map.note_to_pad[osc->midi.note_number] = osc;
  }

  for (auto cymbal : pads.cymbals) {
    midi_map.note_to_pad[cymbal->midi.note_number] = cymbal;
  }

  for (auto audio : pads.audio_ins) {
    midi_map.note_to_pad[audio->midi.note_number] = audio;
  }
}

std::shared_ptr<Layout> ER1::ER1::get_layout() {
  return layout;
}

std::shared_ptr<StateController> ER1::ER1::make_controller(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<Controller>(io, state, shared_from_this());
}

std::shared_ptr<LayoutUI> ER1::ER1::make_ui(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<UI>(config, io, state, shared_from_this());
}
