#include "anemone/config/layouts/layout.hpp"
#include "anemone/types/io/grid/device/coordinates.hpp"


Conf::LayoutBase::LayoutBase(Conf::Base base) : Conf::Base(base)
{}


grid_region_t Conf::LayoutBase::parse_grid_region(std::string name) {
  auto layout = (*this)["layout"];
  
  if ( layout[name].yml.IsNull() ) throw("grid region '" + name + "' undefined!");
  
  YAML::Node x = layout[name]["x"].yml;
  YAML::Node y = layout[name]["y"].yml;

  unsigned int x_min, x_max, y_min, y_max;

  if ( x.IsScalar() ) {
    x_min = x.as<unsigned int>();
    x_max = x.as<unsigned int>();
  } else {
    x_min = x["min"].as<unsigned int>();
    x_max = x["max"].as<unsigned int>();    
  }

  if ( y.IsScalar() ) {
    y_min = y.as<unsigned int>();
    y_max = y.as<unsigned int>();
  } else {
    y_min = y["min"].as<unsigned int>();
    y_max = y["max"].as<unsigned int>();    
  }

  return {
          .min = { .x = x_min, .y = y_min},
          .max = { .x = x_max, .y = y_max},
  };
}
