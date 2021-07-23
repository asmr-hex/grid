#include <spdlog/spdlog.h>

#include "anemone/plugins/instruments/manager.hpp"


// TODO refactor so that there is no plugins directory anymore...
// refactor code so that instead of a state/controller/ui distinction
// there are only Components (kind of like react components) which contain their
// own state, controllers, and UI renderers...
//
// currently, the Plugins are basically like this idea of self-contained components,
// insofar as they initialize the state, setup controllers, and setup their UI renderers.
//
// will have to give some thought into how to make this the prevailinng paradigm for the
// rest of the code base, but i believe it *would* make development much faster and with less
// boiler plate, not to mention that you wouldn't have to skip around this clumsy directory
// structure (i.e. state/controllers/ui)

InstrumentPluginManager::InstrumentPluginManager(std::shared_ptr<Config> config) {
  // get map of instruments from config
  auto instrument_map = config->at("instruments");

  // parse each instrument config
  for (auto itr : instrument_map.yml) {
    auto instrument_name = itr.first.as<std::string>();
    auto instrument_config_path = config->get_parent_dir(config->path) + itr.second.as<std::string>();

    auto instrument_config = std::make_shared<Config>(instrument_config_path);

    auto instrument = std::make_shared<InstrumentPlugin>(instrument_name, instrument_config);

    // assign the default instrument if it is the first in the list
    if (instruments.size() == 0) default_instrument = instrument;

    instruments.push_back(instrument);
    plugins.push_back(instrument);

    spdlog::info("    created instrument '{}'", instrument_name);
  }



  
  // // scan over all instruments listed in the config and check for matches.
  // for (auto itr : instrument_map.yml) {
  //   auto instrument_name = itr.first.as<std::string>();

  //   // create and plugin ER1
  //   if ( instrument_name == "er1" ) {
  //     auto er1 = std::make_shared<ER1::ER1>(config);

  //     if (instruments.size() == 0) default_instrument = er1;
      
  //     instruments.push_back(er1);
  //     plugins.push_back(er1);
      
  //     spdlog::info("    created instrument '{}'", instrument_name);
  //   }
  //   // creat and plugin MICROGRANNNY
  //   else if ( instrument_name == "microgranny" ) {
  //     auto microgranny = std::make_shared<MicroGranny::MicroGranny>(config);

  //     if (instruments.size() == 0) default_instrument = microgranny;
      
  //     instruments.push_back(microgranny);
  //     plugins.push_back(microgranny);
      
  //     spdlog::info("    created instrument '{}'", instrument_name);
  //   }
  // }
}
