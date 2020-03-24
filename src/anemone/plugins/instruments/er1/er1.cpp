#include "anemone/plugins/instruments/er1/er1.hpp"

ER1::ER1::ER1(std::shared_ptr<Config> config)
  : Plugin(config),
    Instrument(create_instrument(InstrumentName::ER1, config, std::make_shared<ER1Layout>(ER1Layout(config))))
{
  
}

std::shared_ptr<Layout> ER1::ER1::get_layout() {
  return layout;
}

std::shared_ptr<StateController> ER1::ER1::make_controller(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<Controller>(io, state, shared_from_this());
}

std::shared_ptr<LayoutUI> ER1::ER1::make_ui(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<UI>(config, io, state, shared_from_this());
}
