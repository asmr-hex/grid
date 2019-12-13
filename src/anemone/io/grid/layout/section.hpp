#ifndef IO_GRID_SECTION_H
#define IO_GRID_SECTION_H

#include "anemone/io/observable.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/event.hpp"


class GridSection : public Observable<grid_event_t> {
public:
  GridSectionName name;

  GridSection(GridSectionName name, const grid_region_t& region)
    : name(name), region_m(region) {};

  unsigned int width();
  unsigned int height();
  unsigned int size();
  grid_region_t region() { return region_m; };

  // should the below be protected? i.e. will anyone have to use them except for derived classes?
  grid_coordinates_t coordinates_of(unsigned int index);
  std::vector<grid_coordinates_t> coordinates_in();
  std::vector<grid_coordinates_t> coordinates_in(unsigned int start, unsigned int end);

  bool contains(const grid_coordinates_t& c);
  
  unsigned int index_of(const grid_coordinates_t& coordinates);
  
private:
  grid_region_t region_m;
  
  unsigned int width_m  = 0;
  unsigned int height_m = 0;
  unsigned int size_m   = 0;
};
  
#endif
