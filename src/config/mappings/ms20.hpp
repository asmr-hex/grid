#ifndef MS20_MAPPING_H
#define MS20_MAPPING_H

#include "types.hpp"

// NOTE: instrument mappings are relative to the "instrument_region"
// defined in the controller mapping.
struct MS20Mapping {
  mapping_range_2d_t bank_select;
};

#endif
