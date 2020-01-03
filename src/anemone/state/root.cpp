#include "anemone/state/root.hpp"


State::Root::Root() {
  sequencer = State::Sequencer();

  // instantiate queue
  queue = std::make_unique<Queue<action_t> >();
  
  // instantiate root composite state
  state = rx::State<root_t>
    ::with_actions<action_t>
    ::compose<sequencer_t>
    ([] (sequencer_t s) -> root_t {
       return {
               .sequencer           = s,
       };
     }, sequencer.state);

  // begin listening for inbound actions
  listen_for_actions();
}


void State::Root::listen_for_actions() {
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
