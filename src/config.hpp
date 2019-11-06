#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>


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
class Config {
public:
  Config(std::string config_path) {
    // read in yaml configuration file
    YAML::Node conf = YAML::LoadFile(config_path);

    // assign to config struct
    instruments = conf["instruments"].as< std::vector<std::string> >();
  };

  std::vector<std::string> instruments;  
};

#endif
