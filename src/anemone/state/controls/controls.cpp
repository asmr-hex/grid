#include "anemone/state/controls/controls.hpp"


GlobalControls::GlobalControls(std::shared_ptr<Config>)
  : shift(rx::behavior<bool>(false)),
    bpm(rx::behavior<double>(120))
{}

void GlobalControls::set_shift(bool on) {
  shift.get_subscriber().on_next(on);
}

void GlobalControls::set_bpm(double b) {
  bpm.get_subscriber().on_next(b);
}
