#include "anemone/action/dispatcher.hpp"


Dispatcher::Dispatcher(std::shared_ptr<Queue<action_t> > q) : queue(q) {};


void Dispatcher::dispatch(const action_t& action) {
  queue->push(action);
}

void Dispatcher::dispatch(action_t&& action) {
  queue->push(std::move(action));
}

void Dispatcher::dispatch(std::vector<action_t> actions) {
  for (auto action : actions) {
    queue->push(std::move(action));
  } 
}

void Dispatcher::dispatch_wait(action_t action) {
  // TODO....
}

