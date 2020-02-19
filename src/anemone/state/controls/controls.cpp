#include "anemone/state/controls/controls.hpp"


GlobalControls::GlobalControls(std::shared_ptr<Config>)
  : shift(rx::behavior<bool>(false))
{}

void GlobalControls::set_shift(bool on) {
  shift.get_subscriber().on_next(on);
}
