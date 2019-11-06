#ifndef SP404_H
#define SP404_H

#include "instrument.hpp"


class SP404 : public Instrument {
public:
  SP404() : Instrument() {};

  static std::string name() {
    return "sp404";
  }
};

#endif
