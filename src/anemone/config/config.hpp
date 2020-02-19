#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layouts.hpp"


class Config : public Conf::Base {
public:
  Conf::Layouts layouts;

  struct {
    
    struct {
      std::vector<std::string> in;
      std::vector<std::string> out;
    } midi;
    
    std::string grid = "/dev/tty.usbserial-m1000843";
  } ports;
  
  Config(std::string config_path);
};

#endif
