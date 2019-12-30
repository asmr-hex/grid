#include "anemone/config/config.hpp"


Config::Config(std::string config_path)
  : Conf::Base(config_path),
    layouts((*this)["layouts"]) {

  if ( yml["grid_address"].IsDefined() ) {
    grid_address = yml["grid_address"].as<std::string>();
  }
  
}
