#include "anemone/controllers/part.hpp"


PartController::PartController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // TODO support "wait for end of sequence"

  auto part_select_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::Parts &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) -> grid_section_index {
                return e.index;
              });

  auto bank_select_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::Banks &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) -> grid_section_index {
                return e.index;
              });
  
  auto part_bank_events = part_select_events.combine_latest(bank_select_events);

  // TODO subscribe to combines observables
  part_bank_events.subscribe
    ([state] (std::pair<grid_section_index, grid_section_index> part_bank) {
       // TODO how do we deal with different layouts with different section sizes???
       // TODO calculate part index from part and bank
       auto part_size = state->layout->get_layouts()->sequencer->parts->size();
       auto bank_size = state->layout->get_layouts()->sequencer->banks->size();
     });
}
