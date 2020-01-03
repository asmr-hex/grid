#include "anemone/clock/clock.hpp"
#include "anemone/clock/constants.hpp"

#include <spdlog/spdlog.h>


Clock::Clock(std::shared_ptr<State::Root> state) : state(state) {}

void Clock::start() {

  subscribe<State::root_t>(state, [this] (State::root_t s) {
                                    update_period(s.sequencer.bpm);
                                  });

  spdlog::info("clock:: running");
  std::thread t([this] { step(); });
  t.detach();
}

void Clock::step() {
  while (true) {
    auto t1 = std::chrono::high_resolution_clock::now();

    tick();

    auto t2 = std::chrono::high_resolution_clock::now();
    auto wait = period - std::chrono::duration_cast<std::chrono::microseconds>(t1 - t2);

    std::this_thread::sleep_for(wait);
  }
}

void Clock::tick() {
  broadcast(tick_m);
  tick_m++;
}

void Clock::update_period(float bpm) {
  period = std::chrono::microseconds
    (static_cast<int>((60 * 1000 * 1000)/(bpm * (float)constants::PPQN_MAX)));
}
