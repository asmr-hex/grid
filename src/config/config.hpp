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

  ControllerMapping mappings;
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

    // parse controllers TODO flesh out more
    controllers = conf["controllers"].as< std::vector<std::string> >();

    // parse controller mappings
    parse_mappings(conf);
    
    // parse instruments
    parse_instrument_config(conf["instruments"]);
  }

  void parse_mappings(YAML::Node conf) {
    YAML::Node mapping = get_mapping_node(conf);
    assert(mapping.IsMap());
    
    for (auto itr : mapping) {
      std::string param = itr.first.as<std::string>();
      YAML::Node coordinates = itr.second;

      if (param == "instrument_select") {
        mappings.instrument_select.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.instrument_select.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.instrument_select.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.instrument_select.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "instrument_region") {
        mappings.instrument_region.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.instrument_region.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.instrument_region.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.instrument_region.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "steps") {
        mappings.steps.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.steps.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.steps.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.steps.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "pages") {
        mappings.pages.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.pages.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.pages.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.pages.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "parts") {
        mappings.parts.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.parts.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.parts.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.parts.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "banks") {
        mappings.banks.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.banks.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.banks.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.banks.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "ppqn") {
        mappings.ppqn.x.min = coordinates["x"]["min"].as<unsigned int>();
        mappings.ppqn.x.max = coordinates["x"]["max"].as<unsigned int>();
        mappings.ppqn.y.min = coordinates["y"]["min"].as<unsigned int>();
        mappings.ppqn.y.max = coordinates["y"]["max"].as<unsigned int>();
      } else if (param == "shift") {
        mappings.shift.x = coordinates["x"].as<unsigned int>();
        mappings.shift.y = coordinates["y"].as<unsigned int>();
      } else if (param == "play_stop") {
        mappings.play_stop.x = coordinates["x"].as<unsigned int>();
        mappings.play_stop.y = coordinates["y"].as<unsigned int>();
      } else if (param == "record") {
        mappings.record.x = coordinates["x"].as<unsigned int>();
        mappings.record.y = coordinates["y"].as<unsigned int>();
      } else if (param == "last_step") {
        mappings.last_step.x = coordinates["x"].as<unsigned int>();
        mappings.last_step.y = coordinates["y"].as<unsigned int>();
      } else if (param == "tempo") {
        mappings.tempo.x = coordinates["x"].as<unsigned int>();
        mappings.tempo.y = coordinates["y"].as<unsigned int>();
      }
    }
  };
  
  void parse_instrument_config(YAML::Node instruments_node) {
    assert(instruments_node.IsMap());

    for (auto it : instruments_node) {
      std::string instrument_name = it.first.as<std::string>();
      YAML::Node instrument_config = it.second;

      // add to configured instruments
      instruments.push_back(instrument_name);

      if (instrument_name == GR1Config::name()) {
        gr1 = new GR1Config(instrument_config, config_path);
      } else if (instrument_name == ER1Config::name()) {
        er1 = new ER1Config(instrument_config, config_path);
      } else if (instrument_name == MS20Config::name()) {
        ms20 = new MS20Config(instrument_config, config_path);
      } else if (instrument_name == SP404Config::name()) {
        sp404 = new SP404Config(instrument_config, config_path);
      }
    }
  };
};

#endif
