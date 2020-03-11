#include "anemone/types/io/grid/animations/periodic_waveforms.hpp"


animation::SineWave::SineWave(std::chrono::milliseconds period_ms, float phase)
  : period(period_ms.count()),
    phase(phase * period_ms.count())
{}

unsigned int animation::SineWave::compute(std::chrono::milliseconds time_step) {
  auto t = time_step.count();
  
  return static_cast<unsigned int>( 15 * ((std::sin( (t + phase) * (1/period) * 2 * M_PI ) * 0.5) + 0.5));
}


animation::Blink::Blink(std::chrono::milliseconds period, unsigned int high, unsigned int low, float duty_cycle)
  : period(period.count()),
    duty_cycle(duty_cycle),
    high(high),
    low(low)
{}

unsigned int animation::Blink::compute(std::chrono::milliseconds time_step) {
  auto t = time_step.count();

  return (t % static_cast<int>(period)) > (period * duty_cycle) ? high : low;
}
