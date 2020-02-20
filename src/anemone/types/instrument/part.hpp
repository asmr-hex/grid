/**
 * @file   types/instrument/part.hpp
 * @brief  Part related types.
 * @author coco
 * @date   2020-02-20
 *************************************************/


#ifndef ANEMONE_TYPES_INSTRUMENT_PART_H
#define ANEMONE_TYPES_INSTRUMENT_PART_H

/// @brief type alias for part index.
typedef unsigned int part_idx_t;

/// @brief type alias for part index.
typedef unsigned int bank_idx_t;

/// @brief bank relative part index.
///
/// @remark
/// since the parts are bucketed into banks, this identifies the part index
/// relative to the bank it is contained in. the conversion from part_idx_t
/// to part_bank_idx_t requires that we know the size of each bank, which is
/// only determinable given a runtime configuration of how the grid layout is
/// organized.
struct part_bank_idx_t {
  unsigned int part;
  unsigned int bank;
};

#endif
