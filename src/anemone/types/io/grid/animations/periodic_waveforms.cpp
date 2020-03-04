#include "anemone/types/io/grid/animations/periodic_waveforms.hpp"


animation::SineWave::SineWave(std::chrono::milliseconds period_ms, float phase)
  : period(period_ms.count()),
    phase(phase * period_ms.count())
{}

unsigned int animation::SineWave::compute(std::chrono::milliseconds time_step) {
  auto t = time_step.count();
  
  return static_cast<unsigned int>( 15 * ((std::sin( (t + phase) * (1/period) * 2 * M_PI ) * 0.5) + 0.5));
}
