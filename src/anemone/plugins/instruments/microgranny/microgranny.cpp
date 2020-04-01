#include "anemone/plugins/instruments/microgranny/microgranny.hpp"


MicroGranny::MicroGranny::MicroGranny(std::shared_ptr<Config> config)
  : Plugin(config),
    Instrument(create_instrument(InstrumentName::MICROGRANNY, config, std::make_shared<MicroGrannyLayout>(MicroGrannyLayout(config))))
{
  // configure microgranny here?
}

std::shared_ptr<Layout> MicroGranny::MicroGranny::get_layout() {
  return layout;
}

std::shared_ptr<StateController> MicroGranny::MicroGranny::make_controller(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<Controller>(io, state, shared_from_this());
}

std::shared_ptr<LayoutUI> MicroGranny::MicroGranny::make_ui(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<UI>(config, io, state, shared_from_this());
}
