#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "base.hpp"
#include "gr1.hpp"
#include "er1.hpp"
#include "ms20.hpp"
#include "sp404.hpp"

#include "mappings/controller.hpp"

#include "../instruments/gr1.hpp"
#include "../instruments/er1.hpp"
#include "../instruments/ms20.hpp"
#include "../instruments/sp404.hpp"


/*
  sequencer configuration.

  this configuration is provided via a YAML file and describes settings for
  the sequencer, which instruments to use, and which controllers are being used.

  here is an example configuration file,

  - bpm: 120.0
  instruments:
  - er-1
  - ms20
  - sp404
  controllers:
  - nanokey
  - glove
*/
class Config : public BaseConfig {
public:
  Config(std::string config_path) : BaseConfig(config_path) {
    parse();
  };

  ControllerMapping *mappings;
  std::vector<std::string> instruments;
  std::vector<std::string> controllers;
  
  GR1Config *gr1;
  ER1Config *er1;
  MS20Config *ms20;
  SP404Config *sp404;

private:
  void parse() {
    // read in yaml configuration file
    YAML::Node conf = YAML::LoadFile(config_path);

    // parse controllers
    controllers = conf["controllers"].as< std::vector<std::string> >();
    
    // parse instruments
    parse_instrument_config(conf["instruments"]);
    
  }

  void parse_instrument_config(YAML::Node instruments_node) {
    assert(instruments_node.IsMap());

    for (auto it : instruments_node) {
      std::string instrument_name = it.first.as<std::string>();
      YAML::Node instrument_config = it.second;

      // add to configured instruments
      instruments.push_back(instrument_name);

      if (instrument_name == GR1::name()) {
        gr1 = new GR1Config(instrument_config, config_path);
      } else if (instrument_name == ER1::name()) {
        er1 = new ER1Config(instrument_config, config_path);
      } else if (instrument_name == MS20::name()) {
        ms20 = new MS20Config(instrument_config, config_path);
      } else if (instrument_name == SP404::name()) {
        sp404 = new SP404Config(instrument_config, config_path);
      }
    }
  };
};

#endif
