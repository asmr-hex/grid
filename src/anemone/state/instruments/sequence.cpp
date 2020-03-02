#include "anemone/state/instruments/sequence.hpp"


std::vector<step_event_t> Sequence::get_events_at(granular_step_idx_t step) {
  // passing an empty layers vector gets all layers.
  return get_events_at(step, {});
}

std::vector<step_event_t> Sequence::get_events_at(granular_step_idx_t step,
                                                  const std::vector<step_event_id_t> &layers)
{
  std::vector<step_event_t> step_events;

  collect_events_at(midi_on, step, layers, step_events);
  collect_events_at(midi_off, step, layers, step_events);
  collect_events_at(midi_cc, step, layers, step_events);
  collect_events_at(midi_nrpn, step, layers, step_events);

  return step_events;
}

void Sequence::collect_events_at(sequence_t sequence,
                                 granular_step_idx_t step,
                                 const std::vector<step_event_id_t> &selected_layers,
                                 std::vector<step_event_t>& results)
{
  try {
    auto layers_at_step = sequence.at(step);

    // we are collecting all layers
    if (selected_layers.size() == 0) {
      for (auto itr : layers_at_step) {
        results.push_back(itr.second);
      }
      // return early.
      return;
    }

    // we are collecting specific layers
    for (auto layer_id : selected_layers) {
      try {
        results.push_back(layers_at_step.at(layer_id));
      } catch (std::out_of_range &error) {
        // no such layer at this step...carry on.
      }
    }

  } catch (std::out_of_range &error) {
    // no events at this step...carry on.
  }
}
