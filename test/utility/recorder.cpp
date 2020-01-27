#include "utility/recorder.hpp"


TestOutputRecorder::TestOutputRecorder(std::shared_ptr<BrowserGridDevice> grid_device,
                                       std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > midi_input_devices,
                                       std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > midi_output_devices,
                                       std::shared_ptr<Anemone> anemone)
  : grid_device(grid_device),
    midi_input_devices(midi_input_devices),
    midi_output_devices(midi_output_devices),
    anemone(anemone) {
  // initialize done_recording queue
  done_recording = std::make_shared< Queue<bool> >();
}

void TestOutputRecorder::listen() {
  // subscribe to clock ticks
  subscribe(*anemone->clock);
}

void TestOutputRecorder::record_step_output(std::vector<types::step::idx_t> steps) {
  is_recording = true;

  // create a pointer to this set of steps
  auto unobserved_steps = std::make_shared<std::set<types::step::idx_t> >();
  for (auto step : steps) {
    unobserved_steps->insert();
  }
  
  // set recording handler.
  recording_handler =
    [this] (const types::tick_t& tick) {
      // if we have recorded this tick already, continue
      

      // check tick conditions
    };

  // wait until we have recieved a done recording message.
  while (!done_recording->pop()) {}

  // TODO return results
}

void handle(const types::tick_t& tick) {
  // immediately return if we are not recording.
  if (!is_recording)
    return;

  // okay, we must be recording.

  // call current recording handler
  recording_handler(tick);
}
