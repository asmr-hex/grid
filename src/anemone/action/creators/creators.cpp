#include "anemone/action/creators/creators.hpp"


action::Creators::Creators(std::shared_ptr<IO> io, std::shared_ptr<GridLayouts> layouts)
  : io(io), layouts(layouts) {};
