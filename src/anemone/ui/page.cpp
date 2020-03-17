#include <spdlog/spdlog.h>

#include "anemone/ui/page.hpp"


PageUI::PageUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto rendered_part = state->instruments->rendered.get_observable()
    | rx::map([] (std::shared_ptr<Instrument> rendered_instrument) {
                return rendered_instrument->status.part.under_edit.get_observable();
              })
    | rx::switch_on_next();

  // rendered page observable
  auto rendered_page = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.rendered.get_observable();
              })
    | rx::switch_on_next();

  // // page under edit observable
  // auto under_edit_page_observable = rendered_part_observable
  //   | rx::map([] (std::shared_ptr<Part> rendered_part) {
  //               return rendered_part->page.under_edit.get_observable();
  //             })
  //   | rx::switch_on_next();

  // last page observable
  auto last_page = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->page.last.get_observable();
              })
    | rx::switch_on_next()
    | rx::map([this] (page_idx_t last_page) {
                // side-effects only map

                clear();
                
                // fill pages up to last page.
                std::vector<grid_section_index> indices;
                for (auto i = 0; i <= last_page; i++) { indices.push_back(i); }
                set_leds(indices, led_level.active_pages);
                
                return last_page;
              });
  
  // page in playback observable
  auto page_in_playback = rendered_part
    | rx::map([] (std::shared_ptr<Part> rendered_part) {
                return rendered_part->step.current.get_observable();
              })
    | rx::switch_on_next()  
    | rx::map([state] (granular_step_idx_t granular_step) {
                auto page_size = state->layout->get_layouts()->sequencer->steps->size();

                return granular_to_paged_step(granular_step, page_size).page;
              })
    | rx::distinct_until_changed();
  

  // page ui logic.
  rendered_page.combine_latest(page_in_playback, last_page)
    .subscribe([this] (std::tuple<page_idx_t, page_idx_t, page_idx_t> t) {
                 auto rendered_page    = std::get<0>(t);
                 auto page_in_playback = std::get<1>(t);
                 auto last_page        = std::get<2>(t);

                 // clear previous rendered page when current changes
                 if (previous.rendered_page != rendered_page)
                   set_led(previous.rendered_page,
                           previous.rendered_page > last_page ?
                           led_level.inactive_pages :
                           led_level.active_pages);

                 // clear previous playing page when current changes
                 if (previous.page_in_playback != page_in_playback)
                   set_led(previous.page_in_playback,
                           previous.page_in_playback > last_page ?
                           led_level.inactive_pages :
                           led_level.active_pages);

                 
                 if (page_in_playback == rendered_page) {
                   // if the playing page and rendered pages are overlapping, animate the led
                   add_animation(led_level.animate.rendered_and_playing_overlapping, page_in_playback);
                 } else {
                   // if the current playing & rendered pages are nont the same, but the previous
                   // playing & rendered pages *were* the same, remove the animation!
                   if (previous.page_in_playback == previous.rendered_page)
                     remove_animation(previous.page_in_playback, led_level.active_pages);
                   
                   // set the currently rendered page
                   set_led(rendered_page, led_level.rendered_page);

                   // set the currently playing page
                   set_led(page_in_playback, led_level.playing_page);
                 }

                 // update previous pages.
                 previous.rendered_page = rendered_page;
                 previous.page_in_playback = page_in_playback;
               });
}
