/**
 * @file   io/clock/clock.hpp
 * @brief  IO Clock
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef IO_CLOCK_H
#define IO_CLOCK_H

#include <chrono>
#include <memory>
#include <thread>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


class Clock : rx::subject<tick_t> {
public:
  Clock(std::shared_ptr<State>);

  rx::observable<tick_t> connect();
  
private:
  double bpm;
  std::chrono::microseconds period;
};

#endif
