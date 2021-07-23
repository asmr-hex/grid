#include "anemone/controllers/step.hpp"


StepController::StepController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // TODO NOTE: this subscription raises CPU usage from 0.5 -> 2.0 (we might want to investigate to
  // optimize this a little perhaps...)
  
  // subscribe to clock ticks
  auto tick_events = io->clock_events
    .subscribe
    ([this, io, state]
     (tick_t t) {
       // iterate over instruments
       for (auto itr : state->instruments->by_name) {
         auto instrument = itr.second;

         // if this instrument is not playing, continue
         if ( !instrument->status.is_playing.get_value() ) continue;

         // get the part in playback
         auto part = instrument->status.part.in_playback.get_value();

         // get step section size.
         // TODO idea: conntrollers (and ui eventually) can be organized by specific layout
         // and we can have these controllers store the sizes of relevant sections.
         auto page_size = state->layouts->sequencer->steps->size();
         
         // get granular cursor step
         auto current_granular_step = part->step.current.get_value();
         
         // get current last step (non-granular)
         auto last_step = part->step.last.get_value().to_absolute_idx(page_size);
         
         // get the midi events at the current step
         auto step_events = part->sequence.get_events_at(current_granular_step);
         
         // increment current cursor step
         auto next_granular_step = current_granular_step + part->ppqn.current.get_value();
         
         // the current step is now greater than the last step, cycle back to first step.
         if (current_granular_step > (last_step * PPQN::Max) - 1 ) {
           // this is eht "end-of-sequence"
           next_granular_step = 0;
           
           // TODO update stuff that needs updating on "end-of-sequence".
         }
         
         // update current granular step
         part->step.update_current(next_granular_step);
         
         // check if the next granular step is a multiple of PPQN::Max, which
         // means that this step is "on beat".
         if ( (next_granular_step % PPQN::Max) == 0 ) {
           // make all state changes for this part which need to happen "on beat"
           
           // if there is a pending PPQN change, do it!
           if ( part->ppqn.pending_change.get_value() ) {
             part->ppqn.current.get_subscriber().on_next(part->ppqn.next.get_value());
             part->ppqn.pending_change.get_subscriber().on_next(false);
           }
         }
         
         // if we are following the cursor, update the rendered/under-edit pages
         if (part->page.follow_cursor.get_value() && !part->step.show_last.get_value()) {
           // compute current playing page from granular step
           auto playing_page = granular_to_paged_step(next_granular_step, page_size).page;
           
           if (previous.playing_page != playing_page) {
             // if the page has changed, update the rendered/under_edit pages
             part->page.rendered.get_subscriber().on_next(playing_page);
             part->page.under_edit.get_subscriber().on_next(playing_page);
           }

           previous.playing_page = playing_page;
         }

         // emit midi_events from step_events
         // TODO either we should consolidate these types or figure out a principled
         // way top use them! add better support for them.
         for (auto step_event : step_events) {
           midi_event_t midi_event = { .source = "", .destination = "", .data = step_event.data };

           // stream these notes on the midi event playback for this instrument.
           instrument->playback_midi_events.get_subscriber().on_next(midi_event);
           
           // emit to midi out device(s)
           io->midi->emit(midi_event);
         }         
       } // end instrument loop
     });
}
