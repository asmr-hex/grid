#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

#include <variant>

#include "anemone/action/sequencer.hpp"


using action_t = std::variant<action::update_bpm>;

#endif
