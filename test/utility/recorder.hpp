/**
 * @file   test/utility/recorder.hpp
 * @brief  Output Recorder For Integration Tests
 * @author coco
 * @date   2020-01-25
 *************************************************/


#ifndef TEST_UTILITY_RECORDER_H
#define TEST_UTILITY_RECORDER_H

#include <set>
#include <map>
#include <vector>
#include <functional>

#include "doubles/device/grid.hpp"
#include "doubles/device/midi.hpp"
#include "anemone/anemone.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/util/concurrent_queue.hpp"


/// @brief Records and reports various types of anemone outputs.
///
/// @details
/// Since the test recorder is responsible for recording timed events, it is
/// an observer of clock ticks and uses the `Observer<types::tick_t>` handler
/// to decide what it is going to record/do.
///
class TestOutputRecorder : public Observer<types::tick_t> {
public:
  TestOutputRecorder(std::shared_ptr<BrowserGridDevice>,
                     std::shared_ptr<BrowserMidiDeviceFactory>,
                     std::shared_ptr<Anemone>);

  /// @brief subscribes and listens to clock ticks.
  ///
  /// @remark
  /// This *must* be called after the anemone has been initialized, since it must be the
  /// last subscriber in the clock's array of subscribers to be called on broadcast!
  void listen();
  
  /// @brief Records the midi and grid led outputs on the provided sequence steps.
  ///
  /// @param steps   a vector of steps to record when the cursor is there.
  ///
  /// @details
  /// This method blocks while it records the midi and grid led outputs of the
  /// current instrument and part at the steps provided. It inspects the outputs
  /// of the ....?
  ///
  /// @remark
  /// This method assumes that while it is recording the steps, no further
  /// modifications are being made to the state of the controller (i.e. no
  /// new activated steps or emitted midi events).
  std::vector<std::map<std::string, std::set<midi_event_t> > > record_midi_output(types::step::idx_t last_step);

private:
  std::shared_ptr<BrowserGridDevice> grid_device;
  std::shared_ptr<BrowserMidiDeviceFactory> midi_manager;
  std::shared_ptr<Anemone> anemone;

  bool is_recording;
  std::function<void(const types::tick_t&)> recording_handler;
  std::shared_ptr< Queue<bool> > done_recording;
  
  /// @brief override `Observer<types::tick_t>` handle method for handling timed measurements.
  ///
  virtual void handle(const types::tick_t&) override;

  /// @brief helper function for performing midi output recording.
  ///
  /// todo document params
  void midi_output_record_handler(const types::tick_t& tick,
                                  types::step::idx_t& current_step,
                                  const types::step::idx_t& last_step,
                                  const std::vector<State::instrument_t>& instruments,
                                  const std::vector<types::part::idx_t>& parts,
                                  std::vector<types::step::paged_idx_t>& previous_paged_cursor_idxs,
                                  bool& is_initial_call,
                                  std::vector<std::map<std::string, std::set<midi_event_t> > >& results);
};

#endif
