#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/manager.hpp"


InstrumentPluginManager::InstrumentPluginManager(std::shared_ptr<Config> config) {
  // get map of instruments from config
  auto instrument_map = config->at("instruments");

  // scan over all instruments listed in the config and check for matches.
  for (auto itr : instrument_map.yml) {
    auto instrument_name = itr.first.as<std::string>();

    // create and pluginn ER1
    if ( instrument_name == "er1" ) {
      auto er1 = std::make_shared<ER1::ER1>(config);

      if (instruments.size() == 0) default_instrument = er1;
      
      instruments.push_back(er1);
      plugins.push_back(er1);
      
      spdlog::info("    created instrument '{}'", instrument_name);
    }
    // creat and plugin SP404
    else if ( instrument_name == "sp404" ) {
      // TODO
    }
  }
}
