#ifndef MS20_H
#define MS20_H

#include "instrument.hpp"


class MS20 : public Instrument {
public:
  MS20() : Instrument() {};

  static std::string name() {
    return "ms20";
  }
};

#endif
