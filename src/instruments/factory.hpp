#ifndef INSTRUMENT_FACTORY_H
#define INSTRUMENT_FACTORY_H

#include "instrument.hpp"
#include "ms20.hpp"
#include "sp404.hpp"
#include "er1.hpp"
#include "gr1.hpp"

#include "../config/config.hpp"


class InstrumentFactory {
public:
  InstrumentFactory() {
    add<MS20>("ms20");
    add<SP404>("sp404");
    add<ER1>("er1");
    add<GR1>("gr1");
  };

  Instrument *create(std::string name, Config *config) {
    return constructor_map[name](config);
  };
  
private:
  /*
    this is a map of instrument constructors keyed by their string names.
  */
  std::map<std::string, Instrument*(*)(Config *)> constructor_map;

  /*
    a template function for instantiating derived instrument classes.
  */
  template<typename T> static Instrument *construct(Config *config) {
    return new T(config);
  };

  /*
    resgisters instruments with the global instrument factory.

    each instrument class must be registered.
  */
  template<typename T> void add(std::string name) {
    constructor_map[name] = &(construct<T>);
  };
};

#endif
