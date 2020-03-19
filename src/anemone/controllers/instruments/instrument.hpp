/**
 * @file   controllers/instruments/instrument.hpp
 * @brief  Instrument Controller Base Class
 * @author coco
 * @date   2020-03-17
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_INSTRUMENTS_INSTRUMENT_H
#define ANEMONE_CONTROLLERS_INSTRUMENTS_INSTRUMENT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief Base class for instrument controllers
///
class InstrumentController {
public:
  InstrumentController(std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  rx::observable<grid_event_t> events;
};

#endif
