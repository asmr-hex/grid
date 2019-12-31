#ifndef ANEMONE_H
#define ANEMONE_H

#include <string>
#include <memory>

#include "anemone/config/config.hpp"
#include "anemone/io/io.hpp"
#include "anemone/io/grid/grid.hpp"
#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/midi/device/midi.hpp"
// #include "io/midi/midi.hpp"


class Anemone {
public:
  Anemone(std::string config_path, std::shared_ptr<GridDevice>, std::shared_ptr<MidiDevice>);
  
  void run();

private:
  std::shared_ptr<Config> config;
  std::shared_ptr<IO> io;
  
  struct {
    std::shared_ptr<GridLayout::Sequencer> sequencer;
  } layouts;
  
  // Clock clock;
  // std::shared_ptr<State::Tree::Root> state;
  // std::shared_ptr<Dispatcher> dispatcher;

  void init_state();
};

// #include <map>
// #include <string>

// #include "io/io.hpp"
// #include "config/config.hpp"
// #include "state/state_machine.hpp"
// #include "sequencer/scheduler.hpp"
// #include "instruments/instrument.hpp"
// #include "instruments/factory.hpp"
// #include "animation/animator.hpp"


// // the controller is the top level class and entrypoint for initializing,
// // configuring, and running this musical controller.
// //
// // at a high level, this musical controller consists of:
// //  * I/O manager which maintains connections to connected MIDI/OSC devices
// //  * a collection of instrument classes which maintain and control instrument state
// //  * a state machine which contains the state of the the sequencer and instruments as
// //    well as event handlers for responding to external inputs.
// //  * a scheduler for running the instrument's clock and scheduling sequencing events.
// //
// // TODO (coco|11.19.2019) refactor to reduce abstract layers (e.g. do we need a StateMachine AND Controller classes?)
// class Anemone {
// public:
//   Anemone(std::string config_path, Grid *grid, Midi *midi) : config(config_path) {
//     io = new IO(grid, midi);
//     io->connect();

//     animation = new Animator(io);

//     // TODO pass animation to state and instruments
    
//     initialize_instruments();
    
//     state_machine = new StateMachine(io, &config, instruments, animation);

//     scheduler = new Scheduler(io, state_machine->state, &config, instruments);
//   };
  
//   void run() {
//     animation->run();
//     state_machine->listen();
//     scheduler->run();
//   };

// private:
//   IO *io;
//   Config config;
//   Animator *animation;
//   Scheduler *scheduler;
//   StateMachine *state_machine;
//   std::map<std::string, Instrument*> instruments;

//   void initialize_instruments() {
//     InstrumentFactory factory = InstrumentFactory();

//     for (std::string instrument : config.instruments) {
//       instruments[instrument] = factory.create(instrument, &config, &io, animation);
//     }
//   };
// };

#endif
