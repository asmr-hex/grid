#include "anemone/state/root.hpp"

#include <spdlog/spdlog.h>


State::Root::Root() {
  spdlog::debug("    - constructing root");

  // instantiate queue
  queue = std::make_unique<Queue<action_t> >();

  // get high frequency state ptrs
  std::shared_ptr<State::step_cursors_t> step_cursors_ptr = step_cursors.state->get();

  // instantiate root composite state
  state = rx::State<root_t>
    ::with_actions<action_t>
    ::compose<sequencer_t, instruments_t>
    ([step_cursors_ptr] (sequencer_t s, instruments_t i) -> root_t {
       return {
               .sequencer   = s,
               .instruments = i,
               .step_cursors = step_cursors_ptr,
       };
     }, sequencer.state, instruments.state);
}


void State::Root::listen() {
  std::thread t([this]{
                  while (true) {
                    action_t action = queue->pop();
                    state->reduce(action);
                  }
                });

  t.detach();
}

std::shared_ptr<rx::dag::Observable<State::root_t> > State::Root::get() {
  return state; 
}

void State::Root::send_action(const action_t& action) {
  queue->push(action);
}

void State::Root::send_action(action_t&& action) {
  queue->push(std::move(action));
}

void State::Root::send_action(const high_freq_action_t& action) {
  step_cursors.state->reduce(action);
}

void State::Root::send_action(high_freq_action_t&& action) {
  step_cursors.state->reduce(std::move(action));
}
