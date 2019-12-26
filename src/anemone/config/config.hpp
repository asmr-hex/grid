#ifndef CONFIG_H
#define CONFIG_H

#include <string>


class Config {
public:
  Conf::Layouts layouts;
  Conf::Instruments instruments;
  float bpm = 120.0;

  Config(std::string config_path);
};

#endif
