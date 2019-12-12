#ifndef IO_GRID_H
#define IO_GRID_H

#include "anemone/io/grid/device/grid.hpp"


class Grid : public Observable<grid_event_t>, public Observer<grid_device_event_t> {
public:
  
private:
  GridDevice device;
};

#endif
