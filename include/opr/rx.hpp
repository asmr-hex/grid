/**
 * @file   rx.hpp
 * @brief  Includes all necessary headers from rxcpp/rx.hpp
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef OPR_RX_H
#define OPR_RX_H

#include <rxcpp/rx.hpp>
namespace rx {
  using namespace rxcpp;
  using namespace rxcpp::sources;
  using namespace rxcpp::operators;
  using namespace rxcpp::subjects;
  using namespace rxcpp::util;
  using namespace rxcpp::schedulers;
}

#endif
