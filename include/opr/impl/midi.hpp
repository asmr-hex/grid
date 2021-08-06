/**
 * @file   engine
 * @brief  engine header
 * @author coco
 * @date   2021-07-23
 *************************************************/


#ifndef OPR_MODULES_IMPL_MIDI_H
#define OPR_MODULES_IMPL_MIDI_H

#include <map>
#include <vector>
#include <string>

#include <fruit/fruit.h>

#include "../rx.hpp"
#include "../types.hpp"
#include "../modules"

namespace opr {
  namespace impl {

    template<protocol T>
    class Output {};

    template<>
    class Output<protocol::Midi> {
    public:
      class Device {
      public:
        class RtMidi : public opr::module::Output<protocol::Midi>::Device {
        public:
          virtual void connect() override;
          virtual void emit(midi_data_t) override;
        };
      };
    };

  }
}

#endif
