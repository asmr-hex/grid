#ifndef SP404_H
#define SP404_H

#include "instrument.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"


class SP404 : public Instrument {
public:
  SP404(Config *config, IO *io) : Instrument(config, io) {};
};

#endif
