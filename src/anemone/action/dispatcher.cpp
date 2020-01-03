#include "anemone/action/types.hpp"
#include "anemone/action/dispatcher.hpp"

#include <spdlog/spdlog.h>


Dispatcher::Dispatcher(std::shared_ptr<rx::Root<action_t> > root) : root(root) {}

void Dispatcher::connect() {
  root->listen();
  spdlog::info("  connected -> state to dispatcher");
}

void Dispatcher::hydrate() {
  root->send_action(action::hydrate_state{});
}

void Dispatcher::dispatch(const action_t& action) {
  root->send_action(action);
}

void Dispatcher::dispatch(action_t&& action) {
  root->send_action(std::move(action));
}

void Dispatcher::dispatch(std::vector<action_t> actions) {
  for (auto action : actions) {
    root->send_action(std::move(action));
  } 
}

void Dispatcher::dispatch_wait(action_t action) {
  // TODO....
}

