#include "anemone/state/selectors/pages.hpp"
#include "anemone/state/selectors/parts.hpp"


types::page::idx_t get_page_under_edit(State::root_t state) {
  // fetch currently rendered part
  auto rendered_part = get_rendered_part(state);

  return rendered_part.page.under_edit;
}

unsigned int get_steps_page_size(std::shared_ptr<IO> io, std::shared_ptr<GridLayouts> layouts) {
  switch (io->grid->layout.name()) {
  case LayoutName::SequencerAndInstrument:
    return layouts->sequencer->steps->size();
    break;
  case LayoutName::ParallelSequencer:
    return 0; // TODO implement this....
    break;
  }
}
