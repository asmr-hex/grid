#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layouts.hpp"


class Config : public Conf::Base {
public:
  Conf::Layouts layouts;

  struct {
    
    struct {
      std::string in;
      std::string out;
    } midi;
    
    std::string grid = "/dev/tty.usbserial-m1000843";
  } ports;
  
  Config(std::string config_path);
};

#endif
