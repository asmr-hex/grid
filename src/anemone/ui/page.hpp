/**
 * @file   ui/page.hpp
 * @brief  Page UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_PAGE_H
#define UI_PAGE_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class PageUI : public UIComponent {
public:
  PageUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
private:
  /// @brief values of previous pages
  struct {
    page_idx_t rendered_page    = 0;
    page_idx_t page_in_playback = 0;
  } previous;

  /// @brief led intensity levels for different pages
  struct {
    unsigned int active_pages  = 4;
    unsigned int rendered_page = 15;
    unsigned int playing_page  = 10;

    // TODO add nested animations struct (can we default initialize it?)
    struct {
      std::shared_ptr<animation::Blink> rendered_and_playing_overlapping
      = std::make_shared<animation::Blink>(std::chrono::milliseconds(500), 15, 10);
    } animate
    ;
  } led_level;
};

#endif
