#ifndef CONFIG_LAYOUTS_H
#define CONFIG_LAYOUTS_H

#include "anemone/config/base.hpp"


namespace Conf {

  class Layouts : public Base {
  public:
    Sequencer sequencer;
    Patterns  patterns;

    Layouts(YAML::Node yml);
    Layouts(std::string path);
  };

}

#endif
