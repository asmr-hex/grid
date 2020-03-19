#include "anemone/config/layouts/instruments/er1.hpp"


Conf::Layout::Instrument::ER1::ER1(Conf::Base base) : Conf::LayoutBase(base) {
  OSC1 = GridSection(GridSectionName::OSC1,
                     parse_grid_region("osc1"));
  OSC2 = GridSection(GridSectionName::OSC2,
                     parse_grid_region("osc2"));
  OSC3 = GridSection(GridSectionName::OSC3,
                     parse_grid_region("osc3"));
  OSC4 = GridSection(GridSectionName::OSC4,
                     parse_grid_region("osc4"));
}
