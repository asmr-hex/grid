#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layouts.hpp"


class Config : public Conf::Base {
public:
  Conf::Layouts layouts;
  std::string grid_address = "/dev/tty.usbserial-m1000843";

  Config(std::string config_path);
};

#endif
