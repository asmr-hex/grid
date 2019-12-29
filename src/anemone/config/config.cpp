#include "anemone/config/config.hpp"


Config::Config(std::string config_path)
  : Conf::Base(config_path),
    layouts((*this)["layouts"]) {}
