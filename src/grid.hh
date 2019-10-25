#ifndef GRID_H
#define GRID_H

#include <monome.h>


class Grid {
public:
  Grid();
  void discover();
private:

  monome_t *monome;
};

#endif
