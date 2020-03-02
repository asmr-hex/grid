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
