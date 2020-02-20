/**
 * @file   types.hpp
 * @brief  Includes all necessary headers from types
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef ANEMONE_TYPES_H
#define ANEMONE_TYPES_H

// grid types
#include "anemone/types/io/grid/event.hpp"
#include "anemone/types/io/grid/device/event.hpp"
#include "anemone/types/io/grid/device/coordinates.hpp"


// midi types
#include "anemone/types/io/midi.hpp"

// clock types
#include "anemone/types/io/clock/tick.hpp"

// layout types
#include "anemone/types/layout/names.hpp"

// ppqn types
#include "anemone/types/ppqn.hpp"

// instrument types
#include "anemone/types/instrument/names.hpp"
#include "anemone/types/instrument/part.hpp"
#include "anemone/types/instrument/page.hpp"
#include "anemone/types/instrument/sequence.hpp"
#include "anemone/types/instrument/step.hpp"
#include "anemone/types/instrument/step_event.hpp"

#endif
