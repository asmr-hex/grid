#include "anemone/state/instruments/sequence.hpp"


std::vector<step_event_t> Sequence::get_events_at(granular_step_idx_t step) {
  // passing an empty layers vector gets all layers.
  return get_events_at(step, {});
}

std::vector<step_event_t> get_events_at(granular_step_idx_t step,
                                        const std::vector<step_event_id_t> &layers)
{
  std::vector<step_event_t> step_events;

  collect_events_at(midi_on, step, layers, &step_events);
  collect_events_at(midi_off, step, layers, &step_events);
  collect_events_at(midi_cc, step, layers, &step_events);
  collect_events_at(midi_nrpn, step, layers, &step_events);

  return std::move(step_events);
}

void Sequencer::collect_events_at(sequence_t sequence,
                                  granular_step_idx_t step,
                                  const std::vector<step_event_id_t> &layers,
                                  std::vector<step_event_t>& results)
{
  try {
    for (auto s : sequence.at(step)) {
      if (layer.size() == 0) {
        // we are collecting all layers
        for (auto itr : s) {
          results.push_back(itr.second);
        }
      } else {
        // we are only collecting specific layers
        for (auto layer : layers) {
          try {
            results.push_back(s.at(layer));
          } catch (std::out_of_range &error) {
            // no such layer at this step...carry on.
          }
        }
      }
    }
  } catch (std::out_of_range &error) {
    // no events at this step...carry on.
  }
}
