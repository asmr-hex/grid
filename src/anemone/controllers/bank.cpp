#include "anemone/controllers/bank.hpp"


BankController::BankController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto selected_bank_index = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::Banks &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) -> grid_section_index_t {
                return e.index;
              });

  // TODO subscribe to combines observables
  selected_bank_index.subscribe
    ([state] (unsigned int selected_bank_idx) {
       auto rendered_instrument = state->instruments->rendered.get_value();
       auto current_part_idx = rendered_instrument->status.part.under_edit.get_value()->id;
       
       auto part_size = state->layouts->sequencer->parts->size();

       auto part_idx = (selected_bank_idx * part_size) + (current_part_idx % part_size);
       auto part = rendered_instrument->parts[part_idx];

       rendered_instrument->status.part.under_edit.get_subscriber().on_next(part);
       rendered_instrument->status.bank.under_edit.get_subscriber().on_next(selected_bank_idx);
     });
}
