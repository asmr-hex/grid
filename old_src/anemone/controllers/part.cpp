#include "anemone/controllers/part.hpp"


PartController::PartController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // TODO support "wait for end of sequence"

  auto selected_part_index = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == "Parts" &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) -> grid_section_index_t {
                return e.index;
              });

  // TODO subscribe to combines observables
  selected_part_index.subscribe
    ([state] (unsigned int selected_part_idx) {
       auto rendered_instrument = state->instruments->rendered.get_value();
       auto current_bank = rendered_instrument->status.bank.under_edit.get_value();
       
       auto part_size = state->layouts->sequencer->parts->size();

       auto part_idx = (current_bank * part_size) + selected_part_idx;
       auto part = rendered_instrument->parts[part_idx];

       rendered_instrument->status.part.under_edit.get_subscriber().on_next(part);
     });
}
