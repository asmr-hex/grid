#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layouts.hpp"


class Config : public Conf::Base {
public:
  Conf::Layouts layouts;

  Config(std::string config_path);
};

#endif
