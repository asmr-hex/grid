#ifndef CONFIG_LAYOUTS_H
#define CONFIG_LAYOUTS_H

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/sequencer.hpp"
#include "anemone/config/layouts/instruments/er1.hpp"



namespace Conf {

  class Layouts : public Base {
  public:
    Layout::Sequencer sequencer;
    // Layout::Patterns  patterns;

    Layout::Instrument::ER1 er1;
    
    Layouts(Base);
  };

}

#endif
