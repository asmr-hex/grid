#ifndef ANIMATION_TYPES_H
#define ANIMATION_TYPES_H

#define _USE_MATH_DEFINES

#include <map>
#include <cmath>

#include <boost/math/common_factor.hpp>


enum WaveformTypes {Sine, Saw, Unit};

// TODO define animation types!
struct waveform {
public:
  int amplitude;       // between 0..15
  struct {
    int period;        // in milliseconds per cycle
    WaveformTypes type;
  } modulator;

  // the pwm, or Pulse Witch Modulator is a step function (on or off)
  // which can have a variable pulse width.
  struct {
    float duty_cycle;  // a percent (pulse width / period)
    int period;        // in milliseconds per cycle
  } pwm;

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
      intensity = compute_intensity(t);

      // add the intensity for this time step into the cache
      cache[t] = intensity;
    }
    
    return intensity;
  };

private:
  std::map<float, int> cache;
  int fundamental_period = -1;

  unsigned int compute_intensity(unsigned int t) {
    // compute modulator
    float mod_a = compute_modulator(t);

    // compute pwm
    float pwm_a = compute_pwm(t);

    return static_cast<unsigned int>(std::floor( amplitude * mod_a * pwm_a ));
  };

  float compute_modulator(unsigned int t) {
    float result;
    
    switch (modulator.type) {
    case Sine:
      result = std::sin( t * ( 1 / modulator.period ) * 2 * M_PI );
      break;
    case Saw:
      result = t / modulator.period;
      break;
    case Unit:
      result = 1;
      break;
    }

    return result;
  };

  float compute_pwm(unsigned int t) {
    float pulse_width = pwm.duty_cycle * pwm.period;

    if (t < pulse_width) return 1;

    return 0;
  };
  
  void compute_fundamental_period() {
    if (fundamental_period != -1) return;
    
    fundamental_period = boost::math::lcm(modulator.period, pwm.period);
  };
};

#endif
