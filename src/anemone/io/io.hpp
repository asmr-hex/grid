#ifndef IO_H
#define IO_H

#include <string>
#include <iostream>

#include "grid/grid.hpp"
#include "midi/midi.hpp"


class IO {
public:
  IO(Grid *grid, Midi *midi) : grid(grid), midi(midi) {};
  
  void connect() {
    // TODO pass addr config in
    grid->connect();
    
    connect_to_midi_out();

    connect_to_midi_in();
  
  };
  
  Grid *grid;
  Midi *midi;
private:
  void connect_to_midi_out() {
    try {
      output.midi = new RtMidiOut();
    } catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }

    // make sure there are ports available
    unsigned int n_ports = output.midi->getPortCount();
    if ( n_ports == 0 ) {
      std::cout << "No ports available!\n";
      delete output.midi;
      exit( EXIT_FAILURE );
    }

    // TODO (coco|31.10.19) maybe add better debugging for port counting later.
    std::string portName;
    for ( unsigned int i=0; i<n_ports; i++ ) {
      try {
        portName = output.midi->getPortName(i);
      }
      catch (RtMidiError &error) {
        error.printMessage();
      }
      std::cout << "  Output Port #" << i+1 << ": " << portName << '\n';
    }
    std::cout << '\n';
    
    
    // great, lets just connect to the first port available
    // TODO (coco|31.10.19) eventually maybe we want to be able to select the port.
    output.midi->openPort(0);
  };

  void connect_to_midi_in() {
    try {
      input.midi = new RtMidiIn();
    } catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }

    // make sure there are ports available
    unsigned int n_ports = input.midi->getPortCount();
    if ( n_ports == 0 ) {
      std::cout << "No ports available!\n";
      delete input.midi;
      exit( EXIT_FAILURE );
    }

    input.midi->openPort(0);
  };
};

#endif
