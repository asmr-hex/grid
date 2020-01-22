#include "anemone/state/selectors/pages.hpp"
#include "anemone/state/selectors/parts.hpp"


types::page::idx_t get_page_under_edit(State::root_t state) {
  // fetch currently rendered part
  auto rendered_part = get_rendered_part(state);

  return rendered_part.page.under_edit;
}
