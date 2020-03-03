/**
 * @file   controllers/play_pause.hpp
 * @brief  Play/Pause Controller
 * @author coco
 * @date   2020-03-02
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_PLAY_PAUSE_H
#define ANEMONE_CONTROLLERS_PLAY_PAUSE_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for the play/pause button.
///
class PlayPauseController {
public:
  PlayPauseController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
