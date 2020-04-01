/**
 * @file   ui/instrument_select.hpp
 * @brief  Instrument Select UI
 * @author coco
 * @date   2020-03-28
 *************************************************/

#ifndef UI_INSTRUMENT_SELECT_H
#define UI_INSTRUMENT_SELECT_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class InstrumentSelectUI : public UIComponent {
public:
  InstrumentSelectUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
