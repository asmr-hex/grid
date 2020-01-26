#include "anemone/config/config.hpp"


Config::Config(std::string config_path)
  : Conf::Base(config_path),
    layouts((*this)["layouts"]) {

  ports.grid     = yml["ports"]["grid"].as<std::string>();
  ports.midi.in  = yml["ports"]["midi"]["in"].as<std::vector<std::string> >();
  ports.midi.out = yml["ports"]["midi"]["out"].as<std::vector<std::string> >();
  
}
