#include "anemone/controllers/sequence.hpp"


SequenceController::SequenceController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::Steps &&
                     e.type    == GridEvent::Pressed;
                 })
    | rx::map([] (grid_event_t e) {
                return e.index;
              });

  auto off_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::Steps &&
                     e.type    == GridEvent::Unpressed;
                 })
    | rx::map([] (grid_event_t e) {
                return e.index;
              });


  on_events
    .subscribe([state] (grid_section_index index) {
                 // get current instrument and part annd page
                 auto rendered_instrument = state->instruments->rendered.get_value();
                 auto rendered_part = rendered_instrument->status.part.under_edit.get_value();
                 auto rendered_page = rendered_part->page.rendered.get_value();
                 auto show_last_step = rendered_part->step.show_last.get_value();

                 // get page size
                 auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                 // construct paged inndex and convert to absolute index and then granular index;
                 paged_step_idx_t selected_paged_step = { .page = rendered_page, .step = index };
                 granular_step_idx_t step = selected_paged_step.to_absolute_idx(page_size);

                 if (show_last_step) {
                   // if we are setting the last step, update the last step!

                   rendered_part->step.last.get_subscriber().on_next(selected_paged_step);
                 } else {
                   // if we are not setting the last step, add a midi note
                   
                   // get the midi notes to put at step
                   auto notes = rendered_instrument->last_midi_notes_played.get_value();

                   rendered_part->sequence.add_midi_note_events_at(selected_paged_step, step, notes);                   
                 }
               });

}
