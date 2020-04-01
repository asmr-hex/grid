/**
 * @file   state/instruments.hpp
 * @brief  Instruments Container Class
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef STATE_INSTRUMENTS_H
#define STATE_INSTRUMENTS_H

#include <map>
#include <memory>

#include "anemone/rx.hpp"
#include "anemone/types.hpp"
#include "anemone/config.hpp"


// forward declare
class InstrumentPluginManager;


class Instruments {
public:
  std::map<InstrumentName, std::shared_ptr<Instrument> > by_name;
  std::map<unsigned int, std::shared_ptr<Instrument> > by_index;

  rx::behavior<std::shared_ptr<Instrument> > rendered;

  Instruments(std::shared_ptr<Config>,
              std::shared_ptr<InstrumentPluginManager>);
  
  void render(InstrumentName);
};

#endif
