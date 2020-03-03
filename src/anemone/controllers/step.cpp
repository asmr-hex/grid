#include "anemone/controllers/step.hpp"


StepController::StepController(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  // TODO NOTE: this subscription raises CPU usage from 0.5 -> 2.0 (we might want to investigate to
  // optimize this a little perhaps...)
  
  // subscribe to clock ticks
  auto tick_events = io->clock_events
    .subscribe([state] (tick_t t) {
                 // when the clock ticks, create an observable of all playing parts.
                 auto playing_parts = rx::observable<>::iterate(state->instruments->by_name)
                   | rx::map([] (std::pair<InstrumentName, std::shared_ptr<Instrument> > p) {
                               return std::get<1>(p);
                             })
                   | rx::filter([] (std::shared_ptr<Instrument> i) {
                                  // filter for only playing instruments
                                  return i->status.is_playing.get_value();
                                })
                   | rx::map([] (std::shared_ptr<Instrument> i) {
                               // retrieve the parts in playback.
                               return i->status.part.in_playback.get_value();
                             })
                   | rx::as_blocking();

                 // advance steps of each part in playback.
                 playing_parts
                   .subscribe([state] (std::shared_ptr<Part> part) {
                                // get step section size.
                                // TODO idea: conntrollers (and ui eventually) can be organized by specific layout
                                // and we can have these controllers store the sizes of relevant sections.
                                auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                                // get granular cursor step
                                auto current_granular_step = part->step.current.get_value();

                                // get current last step (non-granular)
                                auto last_step = part->step.last.get_value().to_absolute_idx(page_size);
  
                                // get the midi events at the current step
                                auto step_events = part->sequence.get_events_at(current_granular_step);

                                // increment current cursor step
                                auto next_granular_step = current_granular_step + part->ppqn.current.get_value();
  
                                // the current step is now greater than the last step, cycle back to first step.
                                if (current_granular_step > (last_step * PPQN::Max) ) {
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
                                
                                // TODO send events to midi output (on IO presumably)
                                // TODO clean up advance step methods on instruments and part
                                // TODO clean up midi_output observable on instruments
                                
                              });
               });
}
