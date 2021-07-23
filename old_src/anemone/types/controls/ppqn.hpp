/**
 * @file   types/controls/ppqn.hpp
 * @brief  PPQN related types.
 * @author coco
 * @date   2020-01-21
 *************************************************/


#ifndef ANEMONE_TYPES_CONTROLS_PPQN_H
#define ANEMONE_TYPES_CONTROLS_PPQN_H

  
/// @brief Pulse Per Quarter Note (ppqn).
enum PPQN {
           One       = 1,
           Two       = 2,
           Four      = 4,
           Eight     = 8,
           Sixteen   = 16,
           ThirtyTwo = 32,
           SixtyFour = 64,
           Max       = 64,
};

#endif
