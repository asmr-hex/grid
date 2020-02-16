#ifndef ER1_H
#define ER1_H

#include "instrument.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"
#include "../animation/animator.hpp"


class ER1 : public Instrument {
public:
  ER1(Config *config, IO *io, Animator *animation) : Instrument(config, io, animation) {};
};

#endif
