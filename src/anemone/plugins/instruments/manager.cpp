#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/manager.hpp"


InstrumentPluginManager::InstrumentPluginManager(std::shared_ptr<Config> config) {
  // get map of instruments from config
  auto instrument_map = config->at("instruments");

  // scan over all instruments listed in the config and check for matches.
  for (auto itr : instrument_map.yml) {
    auto instrument_name = itr.first.as<std::string>();

    // create and plugin ER1
    if ( instrument_name == "er1" ) {
      auto er1 = std::make_shared<ER1::ER1>(config);

      if (instruments.size() == 0) default_instrument = er1;
      
      instruments.push_back(er1);
      plugins.push_back(er1);
      
      spdlog::info("    created instrument '{}'", instrument_name);
    }
    // creat and plugin MICROGRANNNY
    else if ( instrument_name == "microgranny" ) {
      auto microgranny = std::make_shared<MicroGranny::MicroGranny>(config);

      if (instruments.size() == 0) default_instrument = microgranny;
      
      instruments.push_back(microgranny);
      plugins.push_back(microgranny);
      
      spdlog::info("    created instrument '{}'", instrument_name);
    }
  }
}
