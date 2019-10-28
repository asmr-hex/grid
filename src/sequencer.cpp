#include "sequencer.hpp"
#include <chrono>

/*
    min bpm should be like 10 bpm
    min bpm should be like 1 ppqn
    meaning the slowest clock speed will be 1 tick every 6 seconds

    max bpm should be like 300 bpm
    max ppqn should be 64 ppqn
    meaning the fastest clock speed will be 1 tick every 3.125 ms

    avg bpm would be like 130 bpm
    avg ppqn would be like 16 ppqn
    meaning the avg clock speed will be 1 tick every 28.8 ms
 */
Sequencer::Sequencer() {
  bpm = 120.0;
  ppqn = 1;
  tick_count = 0;
  tick_period = Microseconds(static_cast<int>(((60 * 1000 * 1000)/bpm) * ppqn));

  run_dispatcher();

  dispatcher_thread.join();

  std::cout << "done" << std::endl;
}

void Sequencer::dispatch() {
  std::cout << "running dispatch thread" << std::endl;
  
  // we will spin in this loop forever.
  while (true) {
    auto tick = Clock::now();
    auto tock = tick + tick_period;

    while (tick < tock) {
      // do nothing. just spin?

      // TODO: cleep for 3/4 the tick_duration to not churn cpu but wake up in time
      // incase we are preempted -__-
      tick = Clock::now();
    }

    std::cout << "tick "<< tick_count << std::endl;
    tick_count++;
  }
  
  // boost::posix_time::seconds t(2);
  // boost::this_thread::sleep(t);
}

void Sequencer::run_dispatcher() {
  dispatcher_thread = boost::thread(&Sequencer::dispatch, this);
}
