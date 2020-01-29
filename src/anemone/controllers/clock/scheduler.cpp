#include "anemone/controllers/clock/scheduler.hpp"
#include "anemone/state/selectors/instruments.hpp"
#include "anemone/state/selectors/midi_events.hpp"

#include <spdlog/spdlog.h>


ctrl::clock::Scheduler::Scheduler(std::shared_ptr<IO> io,
                                  std::shared_ptr<GridLayouts> layouts,
                                  std::shared_ptr<State::Root> state,
                                  std::shared_ptr<Dispatcher> dispatcher,
                                  std::shared_ptr<Clock> clock)
  : Controller(io, layouts, state, dispatcher, clock) {}


void ctrl::clock::Scheduler::handle(const types::tick_t& tick) {

  // for each instrument in playback
  //   get events for current tick
  //   dispatch all midi events
  //   increment current tick

  std::vector<types::step::event_t> midi_events;
  for ( auto instrument : get_playing_instruments(state)) {
    // get all midi events for current tick
    get_midi_events_for(instrument, state, &midi_events);

    // advance to next step
    dispatch(make_action.advance_step(instrument));    
  }
  
  // emit midi events
  midi->emit(midi_events);
}

