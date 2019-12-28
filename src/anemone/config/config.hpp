#ifndef CONFIG_H
#define CONFIG_H

#include <string>


class Config : public Base {
public:
  Conf::Layouts layouts;
  Conf::Instruments instruments;

  Config(std::string config_path);
};

#endif
