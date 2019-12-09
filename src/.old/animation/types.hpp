#ifndef ANIMATION_TYPES_H
#define ANIMATION_TYPES_H

#define _USE_MATH_DEFINES

#include <iostream>

#include <map>
#include <cmath>
#include <algorithm>

#include <boost/math/common_factor.hpp>


enum WaveformTypes {Sine, Saw, Unit};

struct waveform {
  struct {
    int min = 0;
    int max = 15;
  } amplitude;
  struct Modulator {
    int period;        // in milliseconds per cycle
    WaveformTypes type;
    float phase;
  } modulator;

  // the pwm, or Pulse Witch Modulator is a step function (on or off)
  // which can have a variable pulse width.
  struct PWM {
    float duty_cycle;  // a percent (pulse width / period)
    int period;        // in milliseconds per cycle
    float phase;
  } pwm;
};

class WaveformWrapper {
public:
  WaveformWrapper(waveform w) : wave(w) {};
  
  int compute(unsigned int t) {
    unsigned int intensity;
    
    compute_fundamental_period();

    // where in the cycle does this time step lie?
    t = t % fundamental_period;

    // have we already computed this?
    try {
      intensity = cache.at(t);
    } catch (std::out_of_range &error) {
      // it is not in the cache, we must compute it
      intensity = std::min(compute_intensity(t), (unsigned int)15);

      // add the intensity for this time step into the cache
      cache[t] = intensity;
    }
    
    return intensity;
  };

private:
  waveform wave;
  std::map<float, int> cache = std::map<float, int>();
  int fundamental_period = -1;

  unsigned int compute_intensity(unsigned int t) {
    // compute modulator
    float mod_a = compute_modulator(t);

    // compute pwm
    float pwm_a = compute_pwm(t);
    
    return static_cast<unsigned int>(std::floor( (wave.amplitude.max - wave.amplitude.min + 1) * mod_a * pwm_a ) + wave.amplitude.min);
  };

  float compute_modulator(unsigned int t) {
    float result;

    t = (t + (int)(wave.modulator.phase * wave.modulator.period)) % wave.modulator.period;
    
    switch (wave.modulator.type) {
    case Sine:
      result = (std::sin( (float)t * ( 1 / (float)wave.modulator.period ) * 2 * M_PI ) * 0.5) + 0.5;
      break;
    case Saw:
      result = (float)t / (float)wave.modulator.period;
      break;
    case Unit:
      result = 1;
      break;
    }

    return result;
  };

  float compute_pwm(unsigned int t) {
    float pulse_width = wave.pwm.duty_cycle * (float)wave.pwm.period;

    t = (t + (int)(wave.pwm.phase * wave.pwm.period)) % wave.pwm.period;
    
    if ((float)t < pulse_width) return 1;

    return 0;
  };
  
  void compute_fundamental_period() {
    if (fundamental_period != -1) return;
    
    // ensure that modulator period is set if its Unit waveform
    if (wave.modulator.type == Unit) wave.modulator.period = 1;
    
    fundamental_period = boost::math::lcm(wave.modulator.period, wave.pwm.period);
  };
};

#endif
