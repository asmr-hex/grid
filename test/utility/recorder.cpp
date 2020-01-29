#include "utility/recorder.hpp"

#include "anemone/state/selectors/instruments.hpp"
#include "anemone/state/selectors/parts.hpp"
#include "anemone/state/selectors/pages.hpp"
#include "anemone/state/selectors/step_cursor.hpp"


TestOutputRecorder::TestOutputRecorder(std::shared_ptr<BrowserGridDevice> grid_device,
                                       std::shared_ptr<BrowserMidiDeviceFactory> midi_manager,
                                       std::shared_ptr<Anemone> anemone)
  : grid_device(grid_device),
    midi_manager(midi_manager),
    anemone(anemone) {
  // initialize done_recording queue
  done_recording = std::make_shared< Queue<bool> >();
}

void TestOutputRecorder::listen() {  
  // subscribe to clock ticks
  subscribe(anemone->clock);
}


// thie input is the number of steps to record. these aren't any steps specifically cooresponding
// to specific part steps, but rather the general number of steps we wish to record.
//
// Notably,
// it will record at the 'frame rate' of the fastest step sequence currently playing. In Other words,
// if there is a sequence A with the first 4 steps on and another sequence B with the first 4 steps on but
// playing at doubvle the speed and we specify that we want to record 4 steps, the result will be
// result:
// [Step #]         0  1  2  3   4  5  6  7
// [Sequence A]   | *     *    | *     *
// [Sequence B]   | *  *  *  * | 
//                 ------------
//                  recorded -^   ^- not recorded
//
// additionally, if a step sequence A has length 4 and sequence B has length 2 and they are
// playing at the same speeds and we record 4 steps, the result will be
// result:
// [Step #]           0  1    2  3
// [Sequence A]   | [ a  b    c  d ] |
// [Sequence B]   | [ x  y ][ x  y ] |
//                 ------------------
//                   recorded -^
//
// additionally, the recording of the grid leds will be synchronized to the step cursor of the
// currently rendered part.
// the output is going to be an array where each element is a set of midi out events for each step.
//
// TODO MAYBE ONLY RECORD MIDI OUTPUT HERE....should think of a creative way to measure grid led snapshots
//
std::vector<std::map<std::string, std::set<types::step::event_t> > > TestOutputRecorder::record_midi_output(types::step::idx_t last_step) {
  is_recording = true;

  // get playing instruments and their corresponding playing parts
  auto playing_instruments = get_playing_instruments(anemone->state->state->get());
  std::vector<types::part::idx_t> parts;
  for (auto instrument : playing_instruments) {
    parts.push_back(get_playing_part_id(instrument));
  }

  types::step::idx_t current_step = 0;

  // keep track of the previous step so we don't have to deal with step cursor
  // cycling detection.
  std::vector<types::step::paged_idx_t> previous_paged_cursor_idxs;
  for (int i = 0; i < parts.size(); i++) {
    previous_paged_cursor_idxs.push_back(types::step::paged_idx_t{});
  }

  bool is_initial_call = true;

  std::vector<std::map<std::string, std::set<types::step::event_t> > > results;
  
  // set recording handler.
  recording_handler =
    [this, &current_step, last_step, &playing_instruments, &parts, &previous_paged_cursor_idxs, &is_initial_call, &results]
    (const types::tick_t& tick) {
      midi_output_record_handler(tick,
                                 current_step,
                                 last_step,
                                 playing_instruments,
                                 parts,
                                 previous_paged_cursor_idxs,
                                 is_initial_call,
                                 results);
    };

  // wait until we have recieved a done recording message.
  while (!done_recording->pop()) {}

  return results;
}

void TestOutputRecorder::midi_output_record_handler(const types::tick_t& tick,
                                                    types::step::idx_t& current_step,
                                                    const types::step::idx_t& last_step,
                                                    const std::vector<State::instrument_t>& instruments,
                                                    const std::vector<types::part::idx_t>& parts,
                                                    std::vector<types::step::paged_idx_t>& previous_paged_cursor_idxs,
                                                    bool& is_initial_call,
                                                    std::vector<std::map<std::string, std::set<types::step::event_t> > >& results)
{
  // get current paged_cursors for all playing instruments
  std::vector<types::step::paged_idx_t> current_paged_cursor_idxs;
  for (int i = 0; i < instruments.size(); i++) {
    auto paged_cursor_idx = get_step_cursor_for(instruments[i], parts[i], anemone->state->state->get()).current_page_relative_step;
    current_paged_cursor_idxs.push_back(paged_cursor_idx);
  }

  // if this is the initial call, start recording
  if (is_initial_call) {
    // start recording
    for (auto output_device : midi_manager->get_output_devices()) {
      output_device.second->start_recording();
    }

    // set the previous cursors to the current cursors
    for (int i = 0; i < current_paged_cursor_idxs.size(); i++) {
      previous_paged_cursor_idxs[i] = current_paged_cursor_idxs[i];
    }
    
    is_initial_call = false;

    return;
  }
  
  // okay this isn't the initial call, so the previous_cursors are valid.

  // if any of the previous cursors are different from the current cursors, it is time to
  // cycle the recording process
  bool cycle_recording = false;
  for (int i = 0; i < current_paged_cursor_idxs.size(); i++) {
    if ( !(previous_paged_cursor_idxs[i] == current_paged_cursor_idxs[i]) ) {
      cycle_recording = true;
      break;
    }
  }
  if (!cycle_recording)
    return;

  // okay we need to cycle the recording.

  // first, lets stop the current recording
  std::map<std::string, std::set<types::step::event_t> > step_result;
  for (auto output_device : midi_manager->get_output_devices()) {
    step_result[output_device.first] = output_device.second->stop_recording();
  }

  // store the results
  results.push_back(step_result);

  if (current_step == last_step) {
    // we are done
    done_recording->push(true);
    return;
  }

  // we are not done recording, start recording next step
  for (auto output_device : midi_manager->get_output_devices()) {
    output_device.second->start_recording();
  }

  // set the previous cursors to the current cursors
  for (int i = 0; i < current_paged_cursor_idxs.size(); i++) {
    previous_paged_cursor_idxs[i] = current_paged_cursor_idxs[i];
  }

  current_step++;
}

void TestOutputRecorder::handle(const types::tick_t& tick) {
  // immediately return if we are not recording.
  if (!is_recording)
    return;

  // okay, we must be recording.

  // call current recording handler
  recording_handler(tick);
}
