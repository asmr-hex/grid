/**
 * @file   state/instruments/sequence.hpp
 * @brief  Instrument Sequence Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_SEQUENCE_H
#define STATE_INSTRUMENTS_SEQUENCE_H

#include <memory>
#include <vector>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"


class Sequence {
public:
  sequence_t       midi_on;
  sequence_t       midi_off;
  sequence_t       midi_cc;
  sequence_t       midi_nrpn;
  rendered_steps_t rendered_steps;

  /// @brief added_steps is an observable which sends updates to subscribers who
  /// want to know the most recently added steps.
  rx::subject<paged_step_idx_t> added_steps;

  /// @brief removed_steps is an observable which sends updates to subscribers who
  /// want to know the most recently removed steps.
  rx::subject<paged_step_idx_t> removed_steps;

  /// @brief add sequence layers to the provided step.
  ///
  /// @remark both the paged and granular steps are provided. the paged step is to
  /// populate the rendered steps while the granular step is to insert the layers
  /// into the appropriate sequence.
  ///
  /// @param paged_step        the paged step to render
  /// @param granular_step     the granular step at which to insert the sequence layers.
  /// @param sequence_layers   the sequence layers to add.
  ///
  void add_midi_note_events_at(paged_step_idx_t, granular_step_idx_t, sequence_layer_t);

  /// @brief remove sequence layers from the provided step.
  ///
  /// @remark both the paged and granular steps are provided. the paged step is to
  /// populate the rendered steps while the granular step is to insert the layers
  /// into the appropriate sequence.
  ///
  /// @param paged_step        the paged step to render
  /// @param granular_step     the granular step at which to insert the sequence layers.
  ///
  void remove_midi_note_events_at(paged_step_idx_t, granular_step_idx_t);
  
  /// @brief get step events at the provided step for all layers.
  ///
  /// @param step   a granular step index.
  ///
  /// @return a vector of step events
  ///
  std::vector<step_event_t> get_events_at(granular_step_idx_t);

  /// @brief get step events at the provided step for the provided layer id.
  ///
  /// @param step            a granular step index.
  /// @param step_event_id   the layer id.
  ///
  /// @return a vector of step events
  ///
  std::vector<step_event_t> get_events_at(granular_step_idx_t, const std::vector<step_event_id_t>&);

private:
  void collect_events_at(sequence_t,
                         granular_step_idx_t,
                         const std::vector<step_event_id_t>&,
                         std::vector<step_event_t>&);
};

#endif
