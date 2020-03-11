/**
 * @file   types/io/grid/animations/periodic_waveforms.hpp
 * @brief  IO Grid Periodic Waveform Animation Types
 * @author coco
 * @date   2020-03-04
 *************************************************/


#ifndef TYPES_IO_GRID_ANIMATIONS_PERIODIC_WAVEFORMS_H
#define TYPES_IO_GRID_ANIMATIONS_PERIODIC_WAVEFORMS_H

#include <map>
#include <cmath>
#include <chrono>

#include "anemone/types/io/grid/animation.hpp"


namespace animation {

  class SineWave : public Animation {
  public:
    SineWave(std::chrono::milliseconds period, float phase = 0);

    /// @brief computes the pixel intensity (0-15) at the given time step.
    ///
    /// @param time_step    the time step.
    ///
    /// @return intensity   an unsigned int in [0, 15] representing the pixel value.
    ///
    virtual unsigned int compute(std::chrono::milliseconds time_step) override;

  private:
    /// @brief the period of the sine wave.
    float period;
    
    /// @brief the phase, defined as the percentage of the period [0, 1].
    float phase;

    /// @brief a cache of results to avoid recomputation whennever possible.
    std::map<std::chrono::milliseconds, unsigned int> cache;
  };

  class Blink : public Animation {
  public:
    Blink(std::chrono::milliseconds period, unsigned int high, unsigned int low, float duty_cycle = 0.5);

    /// @brief computes the pixel intensity (0-15) at the given time step.
    ///
    /// @param time_step    the time step.
    ///
    /// @return intensity   an unsigned int in [0, 15] representing the pixel value.
    ///
    virtual unsigned int compute(std::chrono::milliseconds time_step) override;
  private:
    /// @brief period of the blink.
    float period;

    /// @brief the duty cycle of the blink.
    float duty_cycle;
    
    /// @brief high value.
    unsigned int high;

    /// @brief low value.
    unsigned int low;
  };
}

#endif
