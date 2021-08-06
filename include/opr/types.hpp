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

  using step_idx_t = unsigned int;
  using ppqn_t = unsigned int;

  using step_data_id_t = unsigned short;

  enum class protocol { Midi, Osc, Voltage };

  /// @brief midi data type.
  using midi_data_t = std::vector<unsigned char>;

  struct midi_event_t {
    std::string source;
    std::string destination;
    midi_data_t data;
  };

  using midi_port_t = std::string;

  using device_name_t = std::string;
}

#endif
