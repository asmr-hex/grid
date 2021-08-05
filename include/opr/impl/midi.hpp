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

  namespace module {

    namespace impl {

      class RtMidiOutputDevice : public module::Output<protocol::Midi>::Device {
      public:
        INJECT(RtMidiOutputDevice(midi_port_t));
        virtual void emit(midi_data_t) override;
      };

      fruit::Component<module::Output<protocol::Midi>::Device> getOutputMidiDevice(midi_port_t port) {
          return fruit::createComponent()
              .bind<module::Output<protocol::Midi>::Device, RtMidiOutputDevice>()
              .bindInstance(port);
      }
    }
  }
}

#endif
