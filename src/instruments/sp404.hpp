#ifndef SP404_H
#define SP404_H

#include "instrument.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"
#include "../animation/animator.hpp"


class SP404 : public Instrument {
public:
  SP404(Config *config, IO *io, Animator *animation) : Instrument(config, io, animation) {};
};

#endif
