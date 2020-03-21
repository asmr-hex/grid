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

// grid animation types
#include "anemone/types/io/grid/animation.hpp"
#include "anemone/types/io/grid/animations/periodic_waveforms.hpp"

// grid layout types
#include "anemone/types/io/grid/layout/layout.hpp"
#include "anemone/types/io/grid/layout/section.hpp"
#include "anemone/types/io/grid/layout/names.hpp"


// midi types
#include "anemone/types/io/midi/data.hpp"
#include "anemone/types/io/midi/event.hpp"

// clock types
#include "anemone/types/io/clock/tick.hpp"

// global control types
#include "anemone/types/controls/ppqn.hpp"

// instrument types
#include "anemone/types/instrument/names.hpp"
#include "anemone/types/instrument/instrument.hpp"
#include "anemone/types/instrument/part/part.hpp"
#include "anemone/types/instrument/page/page.hpp"
#include "anemone/types/instrument/sequence/sequence.hpp"
#include "anemone/types/instrument/step/step.hpp"
#include "anemone/types/instrument/step/event.hpp"

#endif
