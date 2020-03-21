/**
 * @file   types/instrument/step/step.hpp
 * @brief  Sequence Step related types.
 * @author coco
 * @date   2020-02-20
 *************************************************/


#ifndef ANEMONE_TYPES_INSTRUMENT_SEQUENCE_STEP_STEP_H
#define ANEMONE_TYPES_INSTRUMENT_SEQUENCE_STEP_STEP_H


#include "anemone/types/controls/ppqn.hpp"
#include "anemone/types/instrument/page/page.hpp"


/// @brief type alias for sequence relative step index; a sequential id.
///
/// @remark
/// a step index is used to index a step in a sequence. unlike its granular
/// counter-part, it represents the index that one actually sees on the sequencer
/// and can be thought of as the sequential index of an event within the sequence.
/// This index type is 'sequence' relative because it is agnostic to which page the
/// index lies on.
///
typedef unsigned int step_idx_t;

/// @brief type alias for granular sequence relative indices.
///
/// @remark
/// granular step indices are used to index into sequence steps and are
/// used as the active step maintained within a Part as it is advanced by
/// the sequence scheduler.
/// these indices are "granular" or "fine-grained" because each step represents
/// a 1/constants::PPQN_MAX step (the shortest step possible) and, given how
/// sequences are stored/indexed internally, allows for fluid transitions of a
/// Part's effective ppqn.
/// Like its non-granular counterpart, this index type is 'sequence' relative which
/// means it is agnostic to which page the index lies on.
///
typedef unsigned int granular_step_idx_t;

/// @brief type alias for page relative step index.
///
/// @remark
/// This type is used when we are only concerned with the index of a step relative
/// to the page it is on.
///
typedef unsigned int page_relative_step_idx_t;
    
/// @brief paged step index. provides the page index and page-relative step index.
///
/// @details
/// 
struct paged_step_idx_t {
  page_idx_t               page;
  page_relative_step_idx_t step;

  bool operator==(const paged_step_idx_t& rhs) {
    return
      page == rhs.page &&
      step == rhs.step;
  };

  bool operator!=(const paged_step_idx_t& rhs) {
    return
      page != rhs.page ||
      step != rhs.step;
  };

  /// @brief converts a paged_idx_t to a sequence relative (absolute) sequence step index.
  ///
  /// @param page_size   `unsigned int` representing the size of each page.
  ///
  /// @return step_idx_t
  step_idx_t to_absolute_idx(unsigned int page_size) {
    return (page * page_size) + step;
  };
};

/// @brief convert absolute step to paged step.
///
/// @param step        `step_idx_t`.
/// @param page_size   `unsigned int`
///
/// @return `paged_step_idx_t`
///
inline paged_step_idx_t absolute_to_paged_step(step_idx_t step, unsigned int page_size) {
  return { .page = step / page_size,
           .step = step % page_size,
  };
};

/// @brief convert granular step index to a paged step index
///
/// @param granular_step_idx   the granular step to convert
/// @param page_size           `unsigned int` representing the size of each page.
///
/// @return paged_step_idx_t
///
inline paged_step_idx_t granular_to_paged_step(granular_step_idx_t granular_step_idx,
                                               unsigned int page_size)
{
  return absolute_to_paged_step(granular_step_idx / static_cast<unsigned int>(PPQN::Max),
                                page_size);
};

#endif
