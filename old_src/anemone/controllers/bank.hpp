/**
 * @file   controllers/bank.hpp
 * @brief  Bank Controller
 * @author coco
 * @date   2020-03-02
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_BANK_H
#define ANEMONE_CONTROLLERS_BANK_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"


/// @brief An controller for selecting banks.
///
class BankController {
public:
  BankController(std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif
