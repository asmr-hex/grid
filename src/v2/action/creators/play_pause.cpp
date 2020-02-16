#include "anemone/action/creators/creators.hpp"

#include "anemone/types/sequencer.hpp"
#include "anemone/state/instruments/names.hpp"

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/layout/layouts/sequencer.hpp"

#include "anemone/state/selectors/step_cursor.hpp"
#include "anemone/state/selectors/instruments.hpp"
#include "anemone/state/selectors/parts.hpp"
#include "anemone/state/selectors/pages.hpp"
#include "anemone/state/selectors/steps.hpp"

#include <spdlog/spdlog.h>


action::instrument_played_or_paused action::Creators::play_or_pause_rendered_instrument() {  
  // get rendered instrument
  auto rendered_instrument_name = get_rendered_instrument_name(state);

  return {
          .instrument_name = rendered_instrument_name,
  };
}
