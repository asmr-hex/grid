#ifndef CONFIG_LAYOUTS_INSTRUMENTS_ER1_H
#define CONFIG_LAYOUTS_INSTRUMENTS_ER1_H

#include "anemone/config/base.hpp"
#include "anemone/config/layouts/layout.hpp"
#include "anemone/state/layout/section.hpp"


namespace Conf {
  namespace Layout {
    namespace Instrument {

      class ER1 : public LayoutBase {
      public:
        GridSection OSC1;
        GridSection OSC2;
        GridSection OSC3;
        GridSection OSC4;

        ER1(Base);
      };
    
    }
  }
}

#endif
