#include <thread>
#include <chrono>

#include "anemone/util/wait.hpp"


void wait_for(unsigned int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
};
