#include "sequencer.hpp"


Sequencer::Sequencer() {
  bpm = 120.0;
  ppqn = 4;
  tick_count = 0;

  run_dispatcher();

  dispatcher_thread.join();

  std::cout << "done" << std::endl;
}

void Sequencer::dispatch() {
  std::cout << "running dispatch thread" << std::endl;

  boost::posix_time::seconds t(2);
  boost::this_thread::sleep(t);
}

void Sequencer::run_dispatcher() {
  dispatcher_thread = boost::thread(&Sequencer::dispatch, this);
}
