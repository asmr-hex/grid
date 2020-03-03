#include "anemone/controllers/ppqn.hpp"


PPQNController::PPQNController(std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : index_to_ppqn({ { 0, PPQN::One },
                    { 1, PPQN::Two },
                    { 2, PPQN::Four },
                    { 3, PPQN::Eight },
                    { 4, PPQN::Sixteen },
                    { 5, PPQN::ThirtyTwo },
                    { 6, PPQN::SixtyFour },
    })
{
  // filter for only button-press events from the PPQN grid section.
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::PPQN &&
                     e.type    == GridEvent::Pressed;
                 });

  on_events.subscribe([this, state] (grid_event_t e) {
                        auto rendered_instrument = state->instruments->rendered.get_value();
                        auto rendered_part = rendered_instrument->status.part.under_edit.get_value();

                        // when we press a new ppqn, we don't want to immediately change it because
                        // of the way that sequence events are stored/accessed on clock ticks (see
                        // comments in the sequencer step controller for more details). we need to
                        // ensure that the change in PPQN occurs when the `granular_step_idx` of the
                        // part is on a multiple of PPQN::Max. So, here we will set the next ppqn and
                        // a flag that inndicates that there is a pending change. the sequence step
                        // controller will handle the actual transition.

                        // set the previous & next ppqn and set the pending ppqn-change flag to true
                        rendered_part->ppqn.previous.get_subscriber().on_next(rendered_part->ppqn.current.get_value());
                        rendered_part->ppqn.next.get_subscriber().on_next(index_to_ppqn[e.index]);
                        rendered_part->ppqn.pending_change.get_subscriber().on_next(true);
                      });
}
