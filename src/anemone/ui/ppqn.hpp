/**
 * @file   ui/ppqn.hpp
 * @brief  PPQN UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_PPQN_H
#define UI_PPQN_H

#include <map>
#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class PPQNUI : public UIComponent {
public:
  PPQNUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  std::map<PPQN, unsigned int> ppqn_to_index;

  /// @brief this struct tracks the previous values of the state in order to support
  /// minimal ui re-rendering.
  struct {
    std::shared_ptr<Instrument> rendered_instrument;
    part_idx_t                  rendered_part_idx;
  } previous;
};

#endif
