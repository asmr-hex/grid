#ifndef MS_20_H
#define MS_20_H

#include <iostream>

#include "instrument.hpp"


class MS20 : public Instrument {
public:
  MS20() : Instrument() {
    std::cout << "YAY MS20 was created!" << std::endl;
  };
};

#endif
