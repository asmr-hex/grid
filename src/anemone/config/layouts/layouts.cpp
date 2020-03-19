#include "anemone/config/layouts/layouts.hpp"


Conf::Layouts::Layouts(Conf::Base base)
  : Conf::Base(base),
    sequencer(base["sequencer"]),
    er1(base["instruments"]["er1"])
{}
