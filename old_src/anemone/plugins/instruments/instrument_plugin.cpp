#include "anemone/plugins/instruments/manager.hpp"


InstrumentPlugin::InstrumentPlugin(InstrumentName name, std::shared_ptr<Config> config)
  : Plugin(config),
    Instrument(Instrument::create(name, config))
{}
               
std::shared_ptr<Layout> InstrumentPlugin::get_layout() {
  return layout;
}

std::shared_ptr<StateController> InstrumentPlugin::make_controller(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<Controller>(config, io, state, shared_from_this());
}

std::shared_ptr<LayoutUI> InstrumentPlugin::make_ui(std::shared_ptr<IO> io, std::shared_ptr<State> state) {
  return std::make_shared<UI>(config, io, state, shared_from_this());
}


InstrumentPlugin::Controller::Controller(std::shared_ptr<Config> config,
                                         std::shared_ptr<IO> io,
                                         std::shared_ptr<State> state,
                                         std::shared_ptr<Instrument> instrument)
{
  // TODO parse configuration and add the appropriate controller blocks!
}

InstrumentPlugin::Controller::Block::Block(std::shared_ptr<IO> io,
                                           std::shared_ptr<State> state,
                                           std::shared_ptr<Instrument> instrument)
{
  
}


InstrumentPlugin::UI::UI(std::shared_ptr<Config> config,
                         std::shared_ptr<IO> io,
                         std::shared_ptr<State> state,
                         std::shared_ptr<Instrument> instrument)
{
  // TODO parse configuration and add the appropriate UI blocks!
}
               
InstrumentPlugin::UI::Block::Block(LayoutName layout,
                                   GridSectionName section,
                                   std::shared_ptr<IO> io,
                                   std::shared_ptr<State> state,
                                   std::shared_ptr<Instrument> instrument)
  : UIComponent(layout, section, io, state)
{
  
}               
