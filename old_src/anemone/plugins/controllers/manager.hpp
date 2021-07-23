/**
 * @file   plugins/controllers/manager.hpp
 * @brief  Controllers Plugin Manager Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_CONTROLLERS_PLUGIN_MANAGER_H
#define PLUGINS_CONTROLLERS_PLUGIN_MANAGER_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/plugins/plugin.hpp"

// include all specific controller plugins
// #include "anemone/plugins/controllers/knobs/knobs.hpp"


class ControllerPluginManager {
public:
  ControllerPluginManager(std::shared_ptr<Config>);

  std::vector<std::shared_ptr<Plugin> >    plugins;
};

#endif
