/**
 * @file   ui/play_pause.hpp
 * @brief  Play/Pause UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_PLAY_PAUSE_H
#define UI_PLAY_PAUSE_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class PlayPauseUI : public UIComponent {
public:
  PlayPauseUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
