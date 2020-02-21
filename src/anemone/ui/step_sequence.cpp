#include "anemone/ui/step_sequence.hpp"


StepSequenceUI::StepSequenceUI(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : UIComponent(layout, section, io, state)
{
  auto subscription = rx::composite_subscription();
  // TODO we need to subscribe to an observable, which yields another observable which
  // we want to subscribe to once the original observable updates (but ALSO (AND MOST IMPORTANTLY)
  // UNSUBSCRIBE to the previous second observable).....maybe there is a better way
}
