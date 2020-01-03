#ifndef CLOCK_H
#define CLOCK_H

#include <memory>
#include <thread>
#include <chrono>

#include "anemone/io/observable.hpp"
#include "anemone/rx/observer.hpp"
#include "anemone/state/root.hpp"
#include "anemone/clock/types.hpp"


class Clock : public Observable<tick_t>, rx::Observer {
public:
  Clock(std::shared_ptr<State::Root>);

  void start();
  void step();
private:
  std::chrono::microseconds period;
  tick_t tick_m;

  void tick();
  void update_period(float bpm);
};

#endif
