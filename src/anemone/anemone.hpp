#ifndef ANEMONE_H
#define ANEMONE_H

#include <string>
#include <memory>


#include "anemone/io.hpp"


class Anemone {
public:
  Anemone(std::string config_path,
          std::shared_ptr<GridDevice>);
  
  void run();

  std::shared_ptr<GridDevice> grid_device;
};

#endif
