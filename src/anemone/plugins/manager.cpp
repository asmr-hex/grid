#include "anemone/plugins/manager.hpp"


PluginManager::PluginManager(std::shared_ptr<Config> config)
  : instrument_plugins(std::make_shared<InstrumentPluginManager>(config)),
    controller_plugins(std::make_shared<ControllerPluginManager>(config))
{
  // concatenate all plugins
  plugins.insert(plugins.begin(), instrument_plugins->plugins.begin(), instrument_plugins->plugins.end());
  plugins.insert(plugins.end(), controller_plugins->plugins.begin(), controller_plugins->plugins.end());
}
