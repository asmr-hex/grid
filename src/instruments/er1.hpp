#ifndef ER1_H
#define ER1_H

#include "instrument.hpp"
#include "../config/config.hpp"


class ER1 : public Instrument {
public:
  ER1(Config *config) : Instrument(config) {};
};

#endif
