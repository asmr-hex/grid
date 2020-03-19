/**
 * @file   config/layouts/layout.hpp
 * @brief  Base class for layouts
 * @author coco
 * @date   2020-03-19
 *************************************************/


#ifndef ANEMONE_CONFIG_LAYOUTS_LAYOUT_H
#define ANEMONE_CONFIG_LAYOUTS_LAYOUT_H

#include <string>

#include "anemone/config/base.hpp"
#include "anemone/state/layout/section.hpp"

namespace Conf {

  class LayoutBase : public Base {
  public:
    /// @brief construct layout base.
    ///
    /// @param yaml_base   the base yaml chunk to parse.
    ///
    LayoutBase(Base);

    /// @brief parses grid region from a layout configuration yaml block in
    /// the current config subset.
    ///
    /// @param region_name   a string represeentation to the region name provided in the config.
    ///
    /// @return grid_region
    ///
    grid_region_t parse_grid_region(std::string);

    // TODO (coco|12.29.2019) implement function to ensure there are no overlapping sections!
  };

};


#endif
