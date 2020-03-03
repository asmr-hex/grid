#include <spdlog/spdlog.h>

#include "anemone/ui/ppqn.hpp"


PPQNUI::PPQNUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state),
    ppqn_to_index({ { PPQN::One, 0},
                    { PPQN::Two, 1},
                    { PPQN::Four, 2},
                    { PPQN::Eight, 3},
                    { PPQN::Sixteen, 4},
                    { PPQN::ThirtyTwo, 5},
                    { PPQN::SixtyFour, 6},
      })
{
  auto rendered_part = state->instruments->rendered.get_observable()
    | rx::map([this] (std::shared_ptr<Instrument> rendered_instrument) {
                // TODO question: if we change parts, the ui logic below will not clear the ui from
                // the previos part. would it be weird to do that clearing in here when we *know* the
                // the part has changed...?? Is this a generally pattern i can use when dealing with
                // chained 'switch_on_next()' observables? is this a bad idea? i mean, its not *RX-y*
                // since there are certainly side-effects, but is there an RX-y way to do this?

                // lets try this --^
                if (rendered_instrument != previous.rendered_instrument)
                  clear();
                previous.rendered_instrument = rendered_instrument;

                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([this] (std::shared_ptr<Part> p) {
                // this mapping only has side-effects to clear the ui when the part has changed.
                if (previous.rendered_part_idx != p->id)
                  clear();
                previous.rendered_part_idx = p->id;
                
                return p;
              });
  
  auto current_ppqn = rendered_part
    | rx::map([] (std::shared_ptr<Part> p) {
                
                return p->ppqn.current.get_observable();
              })
    | rx::switch_on_next();

  auto previous_ppqn = rendered_part
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->ppqn.previous.get_observable();
              })
    | rx::switch_on_next();

  auto next_ppqn = rendered_part
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->ppqn.next.get_observable();
              })
    | rx::switch_on_next();

  auto pending_ppqn_change = rendered_part
    | rx::map([] (std::shared_ptr<Part> p) {
                return p->ppqn.pending_change.get_observable();
              })
    | rx::switch_on_next();

  
  current_ppqn.combine_latest(previous_ppqn, next_ppqn, pending_ppqn_change)
    .subscribe([this] (std::tuple<PPQN, PPQN, PPQN, bool> t) {
                 auto current        = std::get<0>(t);
                 auto previous       = std::get<1>(t);
                 auto next           = std::get<2>(t);
                 auto pending_change = std::get<3>(t);

                 // we want to turn off the previous ppqn when there is a change if
                 // the current and previous are different.
                 if (previous != current)
                   turn_off_led(ppqn_to_index[previous]);

                 // always turn on the current
                 turn_on_led(ppqn_to_index[current]);

                 // if there is a pending change, indicate it
                 if (pending_change)
                   set_led(ppqn_to_index[next], 6);
               });
}
