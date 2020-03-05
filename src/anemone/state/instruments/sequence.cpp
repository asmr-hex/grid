#include <spdlog/spdlog.h>

#include "anemone/state/instruments/sequence.hpp"


void Sequence::add_midi_note_events_at(paged_step_idx_t paged_step,
                                       granular_step_idx_t granular_step,
                                       sequence_layer_t sequence_layer)
{
  // add to rendered steps
  try {
    rendered_steps.at(paged_step.page).insert(paged_step.step);  
  } catch (std::out_of_range &error) {
    // no rendered steps previously existed on that page.
    rendered_steps[paged_step.page] = { paged_step.step };
  }

  // broadcast that a rendered step was added
  added_steps.get_subscriber().on_next(paged_step);
}

void Sequence::remove_midi_note_events_at(paged_step_idx_t paged_step,
                                          granular_step_idx_t granular_step)
{
  
}

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
