#include "utility/recorder.hpp"

#include "anemone/state/selectors/instruments.hpp"
#include "anemone/state/selectors/parts.hpp"
#include "anemone/state/selectors/pages.hpp"


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
  // get playing instruments and their corresponding playing parts
  auto playing_instruments = get_playing_instruments(anemone->state->get());
  std::vector<types::part::idx_t> parts;
  for (auto instrument : playing_instruments) {
    parts.push_back(get_playing_part_id(instrument));
  }

  record_step_output(playing_instruments, parts, steps);
}

void TestOutputRecorder::record_step_output(std::vector<State::instrument_t instrument> instruments,
                                            std::vector<types::part::idx_t part> parts,
                                            std::vector<types::step::idx_t> steps) {
  is_recording = true;

  // create a pointer to this set of steps
  auto unobserved_steps = std::make_shared<std::set<types::step::idx_t> >();
  for (auto step : steps) {
    unobserved_steps->insert(step);
  }

  // keep track of the previous step so we don't have to deal with step cursor
  // cycling detection.
  types::step::paged_idx_t previous_paged_cursor_idx;
  types::step::idx_t previous_cursor_idx;
  bool initial_recording = true;
  
  // set recording handler.
  recording_handler =
    [this, unobserved_steps, instruments, parts, @previous_paged_cursor_idx, &previous_cursor_idx, &initial_recording]
    (const types::tick_t& tick) {
      // get the current cursor location of selected instrument/part
      State::step_cursor_t cursor = get_step_cursor_for(instrument, part, anemone->state->get());
      types::step::paged_idx_t paged_cursor_idx = cursor.current_page_relative_step;
      unsigned int page_size = get_steps_page_size(anemone->io, anemone->layouts);
      types::step::idx_t cursor_idx = paged_cursor_idx.to_absolute_idx(page_size);

      // if this is the initial time this handler is called, we don't care about stopping
      // recording for the previous step (since its recordin wasn't started)
      if (initial_recording) {
        initial_recording = false;
      } else {
        // was the previous step an unobserved step?
        if (unobserved_steps->find(previous_cursor_idx) != unobserved_steps->end()) {
          // tell the grid and midi doubles to stop recording
          unsigned int led_snapshot = 0;
          // only snapshot grid led if the cursor is on the currently rendered page
          auto led_snapshot = grid->snapshot_led_at({ .layout  = anemone->io->layout.name(),
                                                      .section = GridSectionName::Steps,  // assuming all layouts name their step sequence section this... -__-
                                                      .index   = previous_paged_cursor_idx.step,
            })

          // store results
          
          // now that we have finished recording this step, lets erase it from the set of unobserved steps
          unobserved_steps->erase(previous_cursor_idx);

          // if the set of unobserved_steps is now empty, we are done!
          if (unobserved_steps->empty()) {
            done_recording->push(true);
            return;
          }
        }
      }

      // if we have recorded this tick already, continue
      if (unobserved_steps->find(cursor_idx) == unobserved_steps->end())
        return;

      // okay, we haven't observed the step cursor at this location yet

      // tell the grid and midi doubles to start recording

      // update previous_cursor_idx
      previous_cursor_idx = cursor_idx_t;
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
