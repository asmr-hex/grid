/**
 * @file   controllers/controllers.hpp
 * @brief  Aggregation of Controllers
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_CONTROLLERS_H
#define ANEMONE_CONTROLLERS_CONTROLLERS_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/state.hpp"

#include "anemone/controllers/layout.hpp"
#include "anemone/controllers/shift.hpp"
#include "anemone/controllers/step.hpp"
#include "anemone/controllers/play_pause.hpp"
#include "anemone/controllers/page.hpp"
#include "anemone/controllers/part.hpp"
#include "anemone/controllers/bank.hpp"
#include "anemone/controllers/ppqn.hpp"
#include "anemone/controllers/sequence.hpp"


/// @brief An aggregation of all controllers which respond to i/o events.
///
class Controllers {
public:
  Controllers(std::shared_ptr<IO>, std::shared_ptr<State>);

  /// @brief this method instantiates all controller classes and connects them to
  /// IO and State.
  ///
  /// @remark
  /// Instead of instantiating all controllers within the constructor, i'm choosing
  /// to do it within this method s.t. the calling code can initialize the controllers
  /// when everything else is being initialized, and then connect the controllers after
  /// IO has been connected. It is important to instantiate all controllers after IO has
  /// connected since the observables exposed in IO will not be instantiated until after
  /// IO has connected.
  /// While i could have just instantiated this controllers container after the IO connection,
  /// i chose to introduce this connect method s.t. it is semantically well-ordered in the calling
  /// code (i.e. only "connecting" happens in the "connecting" phase of spinning up the application).
  ///
  void connect();
  
private:
  std::shared_ptr<IO> io;
  std::shared_ptr<State> state;

  std::unique_ptr<LayoutController>      layout;
  std::unique_ptr<ShiftController>       shift;
  std::unique_ptr<StepController>        step;
  std::unique_ptr<PlayPauseController>   play_pause;
  std::unique_ptr<PageController>        page;
  std::unique_ptr<PartController>        part;
  std::unique_ptr<BankController>        bank;
  std::unique_ptr<PPQNController>        ppqn;
  std::unique_ptr<SequenceController>    sequence;
};

#endif
