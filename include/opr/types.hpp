/**
 * @file   types.hpp
 * @brief  internal core types
 * @author coco
 * @date   2021-08-04
 *************************************************/


#ifndef OPR_TYPES_H
#define OPR_TYPES_H

#include <string>
#include <vector>

namespace opr {

  // TODO organize these.
  struct tick_t {};

  typedef unsigned int step_idx_t;
  typedef unsigned int ppqn_t;

  typedef unsigned short step_data_id_t;

  enum class protocol { Midi, Osc, Voltage };

  /// @brief midi data type.
  typedef std::vector<unsigned char> midi_data_t;

  struct midi_event_t {
    std::string source;
    std::string destination;
    midi_data_t data;
  };

  typedef std::string midi_port_t;
}

#endif
