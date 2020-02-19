#include "anemone/anemone.hpp"

#include <spdlog/spdlog.h>


Anemone::Anemone(std::string config_path,
                 std::shared_ptr<GridDevice> grid_device)
{
  spdlog::set_level(spdlog::level::info);

  spdlog::info("============= initialization ============");
  
  // initialize configuration
  spdlog::info("  initializing \tconfiguration");
  config = std::make_shared<Config>(config_path);

  // initialize state
  spdlog::info("  initializing \tstate");
  state = std::make_shared<State>(State(config));
  
  // initialize io
  spdlog::info("  initializing \tio");
  io = std::make_shared<IO>(IO(config, grid_device, state->layout));
  // io = std::make_shared<IO>(IO(config, grid_device, midi_device_factory, {layouts->sequencer}));

}

void Anemone::run() {
  spdlog::info("============= connecting ================");
  // grid_device->connect("/dev/tty.usbserial-m1000843");

  // auto obs = grid_device->listen()
  //   | rxcpp::operators::filter([] (grid_device_event_t e) {
  //                                return e.type == GridDeviceEvent::ButtonDown;
  //                              });
  // obs.subscribe([] (grid_device_event_t event) { spdlog::info("ok {} : ({}, {})", event.type, event.x, event.y); });
  
  io->connect();
  io->grid_events.subscribe([] (grid_event_t e) {
                              spdlog::info("layout {}, section {}, index {}, type {}", e.layout, e.section, e.index, e.type);
                            });
  
  // wait for 20 seconds
  rx::observable<>::timer(std::chrono::milliseconds(20000)).
    subscribe( [&] (long) {});
}
