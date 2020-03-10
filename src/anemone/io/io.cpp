#include "anemone/io/io.hpp"


IO::IO(std::shared_ptr<Config> config,
      std::shared_ptr<GridDevice> grid_device,
      std::shared_ptr<MidiDeviceFactory> midi_device_factory,
       std::shared_ptr<State> state)
{
  clock = std::make_shared<Clock>(state);
  grid = std::make_shared<Grid>(config, grid_device, state->layout);
  midi = std::make_shared<Midi>(config, midi_device_factory);
}

void IO::connect() {
  clock_events = clock->connect();
  grid_events = grid->connect();
  midi_events = midi->connect();
}
