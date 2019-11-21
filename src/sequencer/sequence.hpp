#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "utils.hpp"
#include "types.hpp"


class Sequence {
public:
  // TODO: we will need to refactor this to come up with a way to
  // gracefully handle midi off events when we change sequence length!

  void collect_all_events_at(step_idx_t step, std::vector<step_event_t> *collector) {
    collect_events_from_sequence_at_step(&midi_off_steps, step, collector);
    collect_events_from_sequence_at_step(&midi_on_steps, step, collector);    
    collect_events_from_sequence_at_step(&midi_cc_steps, step, collector);
    collect_events_from_sequence_at_step(&midi_nrpn_steps, step, collector);
  };
  
  void add_midi_note_on_event(step_event_t event, step_idx_t step, unsigned int off_offset) {
    assert(event.protocol == MIDI);

    step_event_t off_event = midi_note_off_from_on_event(event);
    step_idx_t off_step = step + (off_offset * constants::PPQN_MAX);
    
    midi_on_steps[step][event.id] = event;
    midi_off_steps[off_step][off_event.id] = off_event;
    midi_on_to_midi_off[step][event.id] = off_step;
  };

  void remove_step(step_idx_t step) {
    for (auto i : midi_on_steps[step]) {
      remove_midi_off_events_from_midi_on_events(i.first, step);
    }

    midi_on_steps.erase(step);
  };
  
  void remove_midi_note_on_event(event_uid_t id, step_idx_t step) {
    try {
      midi_on_steps[step].erase(id);
      remove_midi_off_events_from_midi_on_events(id, step);
    } catch (std::out_of_range &error) {
      // a step should ALWAYS exist if this is called...so this shouldn't happen...
      // TODO add logging...
    }
  };

  bool is_step_on(unsigned int step, event_uid_t active_layer) {
    try {
      if (active_layer == 0x0000) {
        // check all layers in this step
        midi_on_steps.at(step);
      } else {
        // check this specific layer
        midi_on_steps.at(step).at(active_layer);
      }
      return true;
    } catch (std::out_of_range &error) {
      // step is not on!
      return false;
    }
  };
private:
  sequence_t midi_on_steps;
  sequence_t midi_off_steps;
  sequence_t midi_cc_steps;
  sequence_t midi_nrpn_steps;
  midi_on_to_midi_off_t midi_on_to_midi_off;

  void collect_events_from_sequence_at_step(sequence_t *seq, step_idx_t step, std::vector<step_event_t> *collector) {
    try {
      for (auto i : seq->at(step)) {
        collector->push_back(i.second);
      }
    } catch (std::out_of_range &error) {
      // doesn't exist...carry on.
    }
  };

  void remove_midi_off_events_from_midi_on_events(event_uid_t id, step_idx_t step) {
    try {
      midi_off_steps[midi_on_to_midi_off[step][id]].erase(midi_off_id_from_on_id(id));
      midi_on_to_midi_off[step].erase(id); 
    } catch (std::out_of_range &error) {
      // a step should ALWAYS exist if this is called...so this shouldn't happen...
      // TODO add logging...
    }    
  };
};


#endif
