#ifndef SEQUENCER_CONSTS_H
#define SEQUENCER_CONSTS_H

/* CONSTANTS */
namespace constants {
  enum PPQN {
             One       = 1,
             Two       = 2,
             Four      = 4,
             Eight     = 8,
             Sixteen   = 16,
             ThirtyTwo = 32,
             SixtyFour = 64,
  };
  
  const int PPQN_MAX            = PPQN::SixtyFour;  // pulse per quarter note globally
  const int SEQUENCE_PAGE_SIZE  = 32;  // number of steps in sequence page
  const int SEQUENCE_COL_SIZE   = 16;  // TODO get rid of this and use calculated from mapping
  const int SEQUENCE_ROW_SIZE   =  2;  // TODO get rid of this and use calculated from mapping
}

#endif
