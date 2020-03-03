/**
 * @file   ui/banks.hpp
 * @brief  Bannks UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_BANKS_H
#define UI_BANKS_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class BanksUI : public UIComponent {
public:
  BanksUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  /// @brief this struct tracks the previous values of the state in order to support
  /// minimal ui re-rendering.
  struct {
    std::shared_ptr<Instrument> rendered_instrument;
    bank_idx_t                  rendered_bank;
    bank_idx_t                  playing_bank;
  } previous;
};

#endif
