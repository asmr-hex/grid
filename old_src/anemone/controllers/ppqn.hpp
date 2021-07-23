/**
 * @file   controllers/ppqn.hpp
 * @brief  PPQN Controller
 * @author coco
 * @date   2020-03-02
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_PPQN_H
#define ANEMONE_CONTROLLERS_PPQN_H

#include <map>
#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for selecting ppqn.
///
class PPQNController {
public:
  PPQNController(std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  std::map<unsigned int, PPQN> index_to_ppqn;
};

#endif
