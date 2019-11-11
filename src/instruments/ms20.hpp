#ifndef MS20_H
#define MS20_H

#include "instrument.hpp"
#include "../config/config.hpp"


class MS20 : public Instrument {
public:
  MS20(Config *config) : Instrument(config) {};
};

#endif
