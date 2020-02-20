#include "anemone/io/clock/clock.hpp"


Clock::Clock(std::shared_ptr<State> state) {
  state->controls->bpm.get_observable()
    .subscribe([this] (double b) {
                 bpm = b;
                 period = std::chrono::microseconds
                   (static_cast<int>((60 * 1000 * 1000)/(bpm * (float)PPQN::Max)));
               });
}

rx::observable<tick_t> Clock::connect() {
  // start clock
  // TODO use rx schedulers to delegate this to a thread
  std::thread
    t([this] () {
        while (true) {
          auto t1 = std::chrono::high_resolution_clock::now();

          get_subscriber().on_next(tick_t{});

          auto t2 = std::chrono::high_resolution_clock::now();
          auto wait = period - std::chrono::duration_cast<std::chrono::microseconds>(t1 - t2);

          std::this_thread::sleep_for(wait);
        }
      });

  t.detach();

  return get_observable();
}
