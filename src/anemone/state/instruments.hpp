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

#include "anemone/state/instruments/instrument.hpp"
#include "anemone/state/instruments/er1/er1.hpp"


class Instruments {
public:
  Instruments(std::shared_ptr<Config>);

  rx::behavior<std::shared_ptr<Instrument> > rendered;
  void render(InstrumentName);

  /// @brief midi output observable.
  rx::subject<midi_data_t> midi_output;
  
  std::shared_ptr<ER1> er1;
  
  std::map<InstrumentName, std::shared_ptr<Instrument> > by_name;
};

#endif
