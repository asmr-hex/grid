#include <chrono>

#include <yaml-cpp/yaml.h>

#include "consts.hpp"
#include "sequencer.hpp"
#include "instruments/factory.hpp"

/*
    min bpm should be like 10 bpm
    min bpm should be like 1 ppqn
    meaning the slowest clock speed will be 1 tick every 6 seconds

    max bpm should be like 300 bpm
    max ppqn should be 64 ppqn
    meaning the fastest clock speed will be 1 tick every 3.125 ms

    avg bpm would be like 130 bpm
    avg ppqn would be like 16 ppqn
    meaning the avg clock speed will be 1 tick every 28.8 ms
 */
Sequencer::Sequencer(std::string config_path) : config_path(config_path) {
  // set defaults and constants
  bpm         = defaults::BPM;
  ppqn        = constants::PPQN;
  tick_period = Microseconds(static_cast<int>((60 * 1000 * 1000)/(bpm * (float)ppqn)));
  tick_count  = 0; // TODO (coco|31.10.19) remove this...
 
  configure();

  initialize_instruments();
  
  connect_io();
}

void Sequencer::start() {
  run_dispatcher();
}

void Sequencer::configure() {
  // read in yaml configuration file
  YAML::Node conf = YAML::LoadFile(config_path);

  // assign to config struct
  config.bpm = conf["bpm"].as<float>();
  config.instruments = conf["instruments"].as< std::vector<std::string> >();
  config.controllers = conf["controllers"].as< std::vector<std::string> >();
}

void Sequencer::initialize_instruments() {
  InstrumentFactory factory = InstrumentFactory();

  for (std::string instrument : config.instruments) {
    instruments[instrument] = factory.create(instrument);
  }
}

void Sequencer::connect_io() {
  // connect to midi out
  connect_to_midi_out();

  // connect to monome grid
  if( !(monome = monome_open("/dev/tty.usbserial-m1000843")) ) {
    std::cout << "Could not connect to monome grid!\n";
    exit( EXIT_FAILURE );
  }
  std::cout << "CONNECTED TO MONOME!\n";
  
  // TODO: connect to midi in controllers (as in subscribe callbacks to them)
}

/*
  Connects to an available midi out port.
 */
void Sequencer::connect_to_midi_out() {
  try {
    midiout = new RtMidiOut();
  } catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  // TODO (coco|31.10.19) maybe add better debugging for port counting later.
  // unsigned int nPorts = midiout->getPortCount();
  // std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";

  // std::string portName;
  // for ( unsigned int i=0; i<nPorts; i++ ) {
  //   try {
  //     portName = midiout->getPortName(i);
  //   }
  //   catch (RtMidiError &error) {
  //     error.printMessage();
  //   }
  //   std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
  // }
  // std::cout << '\n';

  // make sure there are ports available
  unsigned int n_ports = midiout->getPortCount();
  if ( n_ports == 0 ) {
    std::cout << "No ports available!\n";
    delete midiout;
    exit( EXIT_FAILURE );
  }

  // great, lets just connect to the first port available
  // TODO (coco|31.10.19) eventually maybe we want to be able to select the port.
  midiout->openPort(0);
}

void Sequencer::dispatch_event_loop() {
  std::cout << "running dispatch thread" << std::endl;
  
  // we will spin in this loop forever.
  while (true) {
    auto tick = Clock::now();
    
    // dispatch events for this step.
    dispatch();
 
    auto tock = Clock::now();
    Microseconds remaining_usec = tick_period - std::chrono::duration_cast<Microseconds>(tock - tick);
    
    // auto tock = tick + tick_period;

    // while (tick < tock) {
    //   // do nothing. just spin?

    //   // TODO: cleep for 3/4 the tick_duration to not churn cpu but wake up in time
    //   // incase we are preempted -__-
    //   tick = Clock::now();
    // }

    // sleep for tick period.
    // NOTE: this might suffer from scheduling woes if this thread gets preempted.
    // in that case, a hybrid sleep/tightloop spin approach might be called for. this
    // would churn the cpu a bit, but prevent event timing glitches.

    boost::this_thread::sleep(boost::posix_time::microseconds(remaining_usec.count()));
    
    std::cout << "tick "<< tick_count << std::endl;
    tick_count++;
  }
}

/*
  Begins sequencer event dispatcher in separate thread.

  Currently blocks.
 */
void Sequencer::run_dispatcher() {
  dispatcher_thread = boost::thread(&Sequencer::dispatch_event_loop, this);

  // do we need to do this? i guess this just makes use wait rather than exit immediately
  dispatcher_thread.join();
}

void Sequencer::dispatch() {
  std::vector<step_event_t> current_step_events = fetch_next_step_events();

  for (step_event_t event : current_step_events) {
    switch (event.protocol) {
    case OSC:
      // led x, y, set on/off and also intensity is basically it...
      dispatch_osc(event);
      break;
    case MIDI:
      dispatch_midi(event);
      break;
    } 
  }
}

std::vector<step_event_t> Sequencer::fetch_next_step_events() {
  std::vector<step_event_t> current_step_events;
  for ( auto it : instruments ) {
    Instrument *instrument = it.second;
    Part* part = instrument->get_current_part();
    
    std::vector<step_event_t> new_step_events = part->advance();
    current_step_events.insert(current_step_events.end(),
                               new_step_events.begin(),
                               new_step_events.end());
  }

  return current_step_events;
}

void Sequencer::dispatch_osc(step_event_t event) {
  // parse command and coordinates from event uid
  osc_command_t command = (osc_command_t)(event.id >> 8);
  unsigned int x = event.id >> 4;
  unsigned int y = event.id & 0x0F;

  switch (command) {
  case led_on:
    monome_led_on(monome, x, y);
    break;
  case led_off:
    monome_led_off(monome, x, y);
    break;
  case led_brightness:
    monome_led_level_set(monome, x, y, (unsigned int)(event.data[0]));
    break;
  default:
    std::cout << "Unrecognized OSC command " << command << std::endl;
  }
}

void Sequencer::dispatch_midi(step_event_t event) {
  midiout->sendMessage(&event.data);
}
