#ifndef SP404_H
#define SP404_H

#include "instrument.hpp"
#include "../config/config.hpp"


class SP404 : public Instrument {
public:
  SP404(Config *config) : Instrument(config) {};
};

#endif
