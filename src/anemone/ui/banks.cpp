#include <spdlog/spdlog.h>

#include "anemone/ui/banks.hpp"


BanksUI::BanksUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  // auto rendered_part_observable = state->instruments->rendered.get_observable()
  //   | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
  //               return rendered_instrument->status.part.under_edit.get_observable();
  //             })
  //   | rx::switch_on_next();

  
  // // page ui logic.
  // rendered_page_observable.combine_latest(under_edit_page_observable,
  //                                         last_page_observable,
  //                                         in_playback_page_observable)
  //   .subscribe([this] (std::tuple<page_idx_t, page_idx_t, page_idx_t, page_idx_t> t) {
  //                auto last_page        = std::get<2>(t);
  //                auto page_in_playback = std::get<3>(t);

  //                turn_on_led(page_in_playback);

  //                if (page_in_playback == 0) {
  //                  turn_off_led(last_page);
  //                } else {
  //                  turn_off_led(page_in_playback - 1);
  //                }
  //              });
}
