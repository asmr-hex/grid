#include <spdlog/spdlog.h>

#include "anemone/plugins.hpp"
#include "anemone/state/state.hpp"
#include "anemone/state/layouts/layouts.hpp"


GridLayouts::GridLayouts(std::shared_ptr<Config> config, std::shared_ptr<PluginManager> plugin_manager)
  : sequencer(std::make_shared<GridLayout::Sequencer>(config, plugin_manager)),
    layout_by_name({
                    {sequencer->name, sequencer }
      }),
    current_layout(rx::behavior< std::shared_ptr<Layout> >(sequencer))
{}

void GridLayouts::connect(std::shared_ptr<State> state) {

  spdlog::warn("ABOUT TO CONNECT TO SEQUENCER");
  
  sequencer->connect(state); 
}
