#ifndef TYPES_SEQUENCER_H
#define TYPES_SEQUENCER_H

enum class PPQN {
                 One       = 1,
                 Two       = 2,
                 Four      = 4,
                 Eight     = 8,
                 Sixteen   = 16,
                 ThirtyTwo = 32,
                 SixtyFour = 64,
                 Max       = 64,
};

namespace types {
  
  namespace page {
    typedef unsigned int idx_t;
  }

  namespace step {
    typedef unsigned int idx_t;

    typedef unsigned int granular_idx_t;
    
    struct page_relative_idx_t {
      page::idx_t page;
      idx_t       step;

      bool operator==(const page_relative_idx_t& rhs) {
        return
          page == rhs.page &&
          step == rhs.step;
      };

      idx_t to_absolute_idx(unsigned int page_size) {
        return (page * page_size) + step;
      };
    };
  }
}

#endif
