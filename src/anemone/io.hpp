/**
 * @file   io.hpp
 * @brief  Includes all necessary headers from io
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef ANEMONE_IO_H
#define ANEMONE_IO_H


#include "anemone/io/io.hpp"

#include "anemone/io/observer.hpp"
#include "anemone/io/observable.hpp"

#include "anemone/io/grid/grid.hpp"
#include "anemone/io/grid/event.hpp"
#include "anemone/io/grid/address.hpp"

#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/device/monome.hpp"

// TODO maybe seperate these out into a seperate directory....?
#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/layouts/layouts.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"

#endif
