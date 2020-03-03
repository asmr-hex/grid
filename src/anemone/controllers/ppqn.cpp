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
  auto on_events = io->grid_events
    | rx::filter([] (grid_event_t e) {
                   return
                     e.section == GridSectionName::PPQN &&
                     e.type    == GridEvent::Pressed;
                 });
    
  on_events.subscribe([this, state] (grid_event_t e) {
                        auto rendered_instrument = state->instruments->rendered.get_value();
                        auto rendered_part = rendered_instrument->status.part.under_edit.get_value();

                        // TODO set next ppqn, but for now just directly set the ppqn

                        rendered_part->ppqn.current.get_subscriber().on_next(index_to_ppqn[e.index]);
                      });
}
