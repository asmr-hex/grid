#include "anemone/ui/step_sequence.hpp"


StepSequenceUI::StepSequenceUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  // auto status_subscription = rx::composite_subscription();
  // auto part_subscription = rx::composite_subscription();
  // auto page_subscription = rx::composite_subscription();
  // auto sequence_subscription = rx::composite_subscription();

  // // TODO we need to subscribe to an observable, which yields another observable which
  // // we want to subscribe to once the original observable updates (but ALSO (AND MOST IMPORTANTLY)
  // // UNSUBSCRIBE to the previous second observable).....maybe there is a better way

  // state->instruments->rendered.get_observable()
  //   .subscribe([&] (std::shared_ptr<Instrument> rendered_instrument) {
  //                // TODO if our status subscriber is subscribed, unsubscribe.
                 
  //                // we want to subscribe to the status of this newly rendered instrument
  //                auto status_subscriber = rx::make_subscriber<part_idx_t>
  //                  (status_subscription,
  //                   [&] (part_idxt_t part_idx) {
  //                     // TODO if part_subscription is subscribed, unsubscribe

  //                     auto part = rendered_instrument->parts[part_idx];

  //                     // we want to subscribe to the newly rendered page
  //                     auto page_subscriber = rx::make_subscriber<page_idx_t>
  //                       (page_subscription,
  //                        [&] (page_idx_t page) {
  //                          // if page_subscription is subscribed, unnsubscribe

  //                          auto // subscribe to step
  //                        });
                      
  //                     part->page.rendered.get_observable()
  //                       .subscribe()
  //                   });
  
  //                // get the part under_edit

  //                // if our page subscriber is subscribed, unsubscribe
  //                // subscribe to the rendered page of the part under edit

  //                //
  //              });

  // OR use switch_on_next
  auto rendered_part_observable = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();

  auto rendered_page_observable = rendered_part_observable
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.rendered.get_observable();
              })
    | rx::switch_on_next();

  // TODO do the same for steps and rendered steps --^
}
