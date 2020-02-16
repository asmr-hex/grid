#ifndef CONFIG_LAYOUTS_H
#define CONFIG_LAYOUTS_H

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/sequencer.hpp"


namespace Conf {

  class Layouts : public Base {
  public:
    Layout::Sequencer sequencer;
    // Layout::Patterns  patterns;

    Layouts(Base);
  };

}

#endif
