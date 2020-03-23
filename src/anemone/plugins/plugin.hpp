/**
 * @file   plugins/plugin.hpp
 * @brief  Plugin Base Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_PLUGIN_H
#define PLUGINS_PLUGIN_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/state.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/ui/layout_ui.hpp"
#include "anemone/controllers/controller.hpp"


class Plugin {
public:
  std::shared_ptr<Config> config;

  Plugin(std::shared_ptr<Config> config) : config(config) {};
  virtual ~Plugin() = default;

  virtual std::shared_ptr<Layout> get_layout() = 0;

  virtual std::shared_ptr<StateController>
  make_controller(std::shared_ptr<IO>, std::shared_ptr<State>) = 0;

  virtual std::shared_ptr<LayoutUI>
  make_ui(std::shared_ptr<IO>, std::shared_ptr<State>) = 0;
};

#endif
