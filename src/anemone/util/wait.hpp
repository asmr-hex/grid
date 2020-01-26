#ifndef ANEMONE_UTIL_WAIT_H
#define ANEMONE_UTIL_WAIT_H

#include <thread>
#include <chrono>


/// @brief waits for the specified amount of milliseconds within the current thread.
///
/// @param ms   unsigned int number of milliseconds to wait
///
void wait(unsigned int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

#endif
