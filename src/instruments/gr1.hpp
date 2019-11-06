#ifndef GR1_H
#define GR1_H

#include "instrument.hpp"


class GR1 : public Instrument {
public:
  GR1() : Instrument() {};

  static std::string name() {
    return "gr1";
  }
};

#endif
