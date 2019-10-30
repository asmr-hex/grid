#include "sequencer.hpp"
#include <chrono>

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
Sequencer::Sequencer() {
  bpm = 120.0;
  ppqn = 1;
  tick_count = 0;
  tick_period = Microseconds(static_cast<int>(((60 * 1000 * 1000)/bpm) * ppqn));

  try {
    midiout = new RtMidiOut();
  } catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  unsigned int nPorts = midiout->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";

  std::string portName;
  for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
      portName = midiout->getPortName(i);
    }
    catch (RtMidiError &error) {
      error.printMessage();
    }
    std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
  }
  std::cout << '\n';
  
  run_dispatcher();

  // do we need to do this? i guess this just makes use wait rather than exit immediately
  dispatcher_thread.join();
}

void Sequencer::dispatch_event_loop() {
  std::cout << "running dispatch thread" << std::endl;
  
  // we will spin in this loop forever.
  while (true) {
    auto tick = Clock::now();
    
    // dispatch events for this step.
    dispatch();

    // queue events for next step.
    enqueue_next_step();
    
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

void Sequencer::run_dispatcher() {
  dispatcher_thread = boost::thread(&Sequencer::dispatch_event_loop, this);
}

void Sequencer::dispatch() {
  while (!next_step_events.empty()) {
    step_event event = next_step_events.front();

    switch (event.protocol) {
    case OSC:
      // led x, y, set on/off and also intensity is basically it...
      dispatch_osc(event);
      break;
    case MIDI:
      dispatch_midi(event);
      break;
    }

    next_step_events.pop();
  }
}

void Sequencer::enqueue_next_step() {
  
}

void Sequencer::dispatch_osc(step_event event) {
  
}

void Sequencer::dispatch_midi(step_event event) {
  midiout.sendMessage();
}
