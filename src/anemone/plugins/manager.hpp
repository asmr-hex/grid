/**
 * @file   plugins/manager.hpp
 * @brief  Plugin Manager Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_PLUGIN_MANAGER_H
#define PLUGINS_PLUGIN_MANAGER_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/plugins/instruments/manager.hpp"
#include "anemone/plugins/controllers/manager.hpp"


class PluginManager {
public:
  PluginManager(std::shared_ptr<Config>);

  std::shared_ptr<InstrumentPluginManager> instrument_plugins;
  std::shared_ptr<ControllerPluginManager> controller_plugins;

  std::vector< std::shared_ptr<Plugin> > plugins;
};

#endif
