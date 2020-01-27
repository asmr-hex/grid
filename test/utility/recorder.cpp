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

  // get current instrument/part
  // TODO eventually make these function parameters
  State::instrument_t instrument;
  types::part::idx_t part;
  
  // create a pointer to this set of steps
  auto unobserved_steps = std::make_shared<std::set<types::step::idx_t> >();
  for (auto step : steps) {
    unobserved_steps->insert(step);
  }
  
  // set recording handler.
  recording_handler =
    [this, unobserved_steps, instrument, part] (const types::tick_t& tick) {
      // get the current cursor location of selected instrument/part
      State::step_cursor_t cursor = get_step_cursor_for(instrument, part, anemone->state->get());
      types::step::paged_idx_t paged_cursor_idx = cursor.current_page_relative_step;
      types::step::idx_t cursor_idx = paged_cursor_idx.to_absolute_idx(8); // TODO GET PAGE SIZE!!!!

      // if we have recorded this tick already, continue
      if (unobserved_steps->find(cursor_idx) == unobserved_steps->end())
        return;

      // okay, we haven't observed this one yet and we want to.
      

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
