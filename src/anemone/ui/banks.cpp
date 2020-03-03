#include <spdlog/spdlog.h>

#include "anemone/ui/banks.hpp"


BanksUI::BanksUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_bank_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.bank.under_edit.get_observable();
              })
    | rx::switch_on_next();

  auto playing_bank_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.bank.in_playback.get_observable();
              })
    | rx::switch_on_next();

  
  // banks ui logic.
  rendered_bank_observable.combine_latest(playing_bank_observable)
    .subscribe([this] (std::tuple<bank_idx_t, bank_idx_t> t) {
                 auto rendered_bank = std::get<0>(t);
                 auto playing_bank  = std::get<1>(t);

                 turn_on_led(rendered_bank);

               });
}
