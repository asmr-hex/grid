#include "anemone/io/io.hpp"


IO::IO(std::shared_ptr<Config> config,
      std::shared_ptr<GridDevice> grid_device,
      // std::shared_ptr<MidiDeviceFactory> midi_device_factory,
       std::shared_ptr<LayoutContext> layout) {
  // create Grid & Midi object wrappers over their respective device objects
  grid = std::make_shared<Grid>(config, grid_device, layout);
  // midi = std::make_shared<Midi>(config, midi_device_factory);
}

void IO::connect() {
  grid_events = grid->connect();
  // midi->connect();
}
