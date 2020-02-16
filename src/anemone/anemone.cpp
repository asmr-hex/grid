#include "anemone/anemone.hpp"
#include "anemone/io/grid/device/events.hpp"

#include <spdlog/spdlog.h>


Anemone::Anemone(std::string config_path,
                 std::shared_ptr<GridDevice> grid_device)
  : grid_device(grid_device)
{
  spdlog::set_level(spdlog::level::info);

  spdlog::info("============= initialization ============");
    
  // initialize io
  spdlog::info("  initializing \tio");
  // io = std::make_shared<IO>(IO(config, grid_device, midi_device_factory, {layouts->sequencer}));

}

void Anemone::run() {
  spdlog::info("============= connecting ================");
  grid_device->connect("/dev/tty.usbserial-m1000843");

  auto obs = grid_device->get_observable()
    | rxcpp::operators::filter([] (grid_device_event_t e) {
               return e.type == GridDeviceEvent::ButtonDown;
             });
  obs.subscribe([] (grid_device_event_t event) { spdlog::info("{} : ({}, {})", event.type, event.x, event.y); });
  
  grid_device->listen();
}
