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
  
}

#endif
