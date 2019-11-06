#ifndef ER1_H
#define ER1_H

#include "instrument.hpp"


class ER1 : public Instrument {
public:
  ER1() : Instrument() {};

  static std::string name() {
    return "er1";
  }
};

#endif
