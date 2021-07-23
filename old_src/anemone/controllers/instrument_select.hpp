/**
 * @file   controllers/instrument_select.hpp
 * @brief  Instrument Select Controller Class
 * @author coco
 * @date   2020-03-28
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_INSTRUMENT_SELECT_H
#define ANEMONE_CONTROLLERS_INSTRUMENT_SELECT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief instrument select controller
///
class InstrumentSelectController {
public:
  InstrumentSelectController(std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  rx::observable<grid_event_t> events;
};

#endif
