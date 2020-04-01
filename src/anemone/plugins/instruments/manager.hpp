/**
 * @file   plugins/instruments/instrument_plugins.hpp
 * @brief  Instrument Plugin Manager Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_INSTRUMENT_PLUGIN_MANAGER_H
#define PLUGINS_INSTRUMENT_PLUGIN_MANAGER_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"

#include "anemone/plugins/plugin.hpp"

// include all specific instrument plugins
#include "anemone/plugins/instruments/er1/er1.hpp"
#include "anemone/plugins/instruments/microgranny/microgranny.hpp"


class InstrumentPluginManager {
public:
  InstrumentPluginManager(std::shared_ptr<Config>);

  std::shared_ptr<Instrument>               default_instrument;
  std::vector<std::shared_ptr<Instrument> > instruments;
  std::vector<std::shared_ptr<Plugin> >     plugins;
};

#endif
