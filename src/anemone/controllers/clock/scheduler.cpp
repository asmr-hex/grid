#include "anemone/controllers/clock/scheduler.hpp"
#include "anemone/state/selectors/instruments.hpp"

#include <spdlog/spdlog.h>


ctrl::clock::Scheduler::Scheduler(std::shared_ptr<IO> io,
                                  std::shared_ptr<State::Root> state,
                                  std::shared_ptr<Dispatcher> dispatcher,
                                  std::shared_ptr<Clock> clock)
  : Controller(io, state, dispatcher, clock) {}


void ctrl::clock::Scheduler::handle(const tick_t& tick) {
  // advance steps
  spdlog::debug("clock tick: {}", tick);

  // for each instrument in playback
  //   get events for current tick
  //   dispatch all midi events
  //   increment current tick

  for ( auto instrument : get_playing_instruments(state)) {
    dispatch(make_action.advance_step(instrument));
  }
}

