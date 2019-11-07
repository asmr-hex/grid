#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <string>

#include "io/io.hpp"
#include "config/config.hpp"
#include "state/state_machine.hpp"
#include "sequencer/scheduler.hpp"
#include "instruments/instrument.hpp"
#include "instruments/factory.hpp"


class Controller {
public:
  Controller(std::string config_path) : config(config_path) {
    io.connect();

    initialize_instruments();
    
    state_machine = new StateMachine(&io, &config, instruments);

    scheduler = new Scheduler(&io, state_machine->state, instruments);
  };
  void run() {
    state_machine->listen();
    scheduler->run();
  };

private:
  IO io;
  Config config;
  Scheduler *scheduler;
  StateMachine *state_machine;
  std::map<std::string, Instrument*> instruments;

  void initialize_instruments() {
    InstrumentFactory factory = InstrumentFactory();

    for (std::string instrument : config.instruments) {
      instruments[instrument] = factory.create(instrument);
    }
  };
};

#endif
