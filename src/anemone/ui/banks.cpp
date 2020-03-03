#include <spdlog/spdlog.h>

#include "anemone/ui/banks.hpp"


BanksUI::BanksUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_instrument = state->instruments->rendered.get_observable()
    | rx::map([this] (std::shared_ptr<Instrument> rendered_instrument){
                // a side-effect only map. just to clear the ui if necessary
                if (previous.rendered_instrument != rendered_instrument)
                  clear();

                return rendered_instrument;
              });
  
  auto rendered_bank = rendered_instrument
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.bank.under_edit.get_observable();
              })
    | rx::switch_on_next();

  auto playing_bank = rendered_instrument
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.bank.in_playback.get_observable();
              })
    | rx::switch_on_next();

  
  // banks ui logic.
  rendered_bank.combine_latest(playing_bank)
    .subscribe([this] (std::tuple<bank_idx_t, bank_idx_t> t) {
                 auto rendered_bank = std::get<0>(t);
                 auto playing_bank  = std::get<1>(t);

                 // turn off the previous bank if it is different from the current bank
                 if (rendered_bank != previous.rendered_bank)
                   turn_off_led(previous.rendered_bank);
                 if (playing_bank != previous.playing_bank)
                   turn_off_led(previous.playing_bank);

                 // always set the current banks
                 set_led(playing_bank, 6);
                 turn_on_led(rendered_bank);

                 // set previous banks
                 previous.rendered_bank = rendered_bank;
                 previous.playing_bank = playing_bank;
               });
}
