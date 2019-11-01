#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

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
struct sequencer_config_t {
  float bpm;
  std::vector<std::string> instruments;
  std::vector<std::string> controllers;
};

#endif
