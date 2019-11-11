#ifndef MS20_H
#define MS20_H

#include "instrument.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"


class MS20 : public Instrument {
public:
  MS20(Config *config, IO *io) : Instrument(config, io) {};
};

#endif
