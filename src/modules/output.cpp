#include <opr/modules>
// #include <opr/impl/midi.hpp>

using namespace opr;
using namespace opr::module;


Output<protocol::Midi>::Output(std::unique_ptr<Output<protocol::Midi>::Device> device)
    : device(std::move(device))
{}


Output<protocol::Midi>::Output(midi_port_t port) {
    // dependency injection!
}

void Output<protocol::Midi>::emit(midi_data_t event) {
  device->emit(event);
}

void Output<protocol::Midi>::emit(std::vector<midi_data_t>& events) {
  for (auto event : events) {
    emit(event);
  }
}
