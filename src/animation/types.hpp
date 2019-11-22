#ifndef ANIMATION_TYPES_H
#define ANIMATION_TYPES_H

#include <map>


enum WaveformTypes {Sine, Saw, Unit};

// TODO define animation types!
struct waveform {
public:
  int amplitude;
  struct {
    float frequency;
    WaveformTypes type;
  } modulator;
  struct {
    float duty_cycle;
    float frequency;
  } step_fn;

  int compute(int time_step) {
    return 0;
  };
  
private:
  std::map<float, int> cache;
};

#endif
