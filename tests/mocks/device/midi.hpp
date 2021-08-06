#ifndef TEST_MOCKS_DEVICE_MIDI_H
#define TEST_MOCKS_DEVICE_MIDI_H

#include <opr/modules>

namespace mock {
  template<opr::protocol T>
  class Output {};

  template<>
  class Output<opr::protocol::Midi> {
  public:
    class Device : public opr::module::Output<opr::protocol::Midi>::Device {
    public:
      virtual void connect() override;
      virtual void emit(opr::midi_data_t) override;
    };
  };
}

#endif
