#include "utility/recorder.hpp"


TestOutputRecorder::TestOutputRecorder(std::shared_ptr<BrowserGridDevice> grid_device,
                                       std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > midi_input_devices,
                                       std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > midi_output_devices,
                                       std::shared_ptr<Anemone> anemone)
  : grid_device(grid_device),
    midi_input_devices(midi_input_devices),
    midi_output_devices(midi_output_devices),
    anemone(anemone) {}

void TestOutputRecorder::record_step_output(std::vector<types::step::idx_t> steps) {
  // TODO ....how are we going to do this...?
};

