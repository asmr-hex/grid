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
}

#endif
