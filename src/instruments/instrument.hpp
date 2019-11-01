// #ifndef INSTRUMENT_H
// #define INSTRUMENT_H

// #include <map>
// #include <string>
// #include <vector>
// #include "../consts.hpp"

// typedef unsigned int event_uid_t; // the first two bytes of a midi command
// // enum osc_command_t { on, off, brightness };

// enum protocol_t { MIDI, OSC };

// // idea, solo notes...
// enum playback_t { playing, stopped, paused, muted, solo };

// struct step_event_t {
//   protocol_t protocol;
//   event_uid_t id;
// };

// /*
//   defines each step in a map, keyed by the step. more space-efficient
//   than storing a sparse vector.

//   the inner map is keyed by the event unique identifier. this way we have
//   constant time look ups for modifying events at certain steps.
//  */
// typedef std::map<unsigned int, std::map<event_uid_t, step_event_t> > sequence_t;

// class Part {
// public:
//   Part(std::string id) : id(id) {
//     // load part file if it exists.
//   };

//   void load(int new_part_idx) {
    
//   }

//   void save() {
    
//   }
  
//   std::vector<step_event_t> advance() {
//     std::vector<step_event_t> next_events;

//     // if playback is not playing, do not advance.
    
//     // how far do we skip ahead for the next active step?
//     int step_size = constants::PPQN / ppqn;

//     // advance to next step
//     active_step += step_size;

//     // if the active step is now greater than the last step, circle back
//     if (active_step > (length * constants::PPQN) - 1) {
//       active_step = 0;
//     }

//     try {
//       // get events from new active step
//       auto step = sequence.at(active_step);
//       for (auto i : step) {
//         // copy sequence event into results
//         next_events.push_back(i.second);
//       }
//     } catch (std::out_of_range &error) {
//       // this step is empty. carry on.
//     }

//     return next_events;
//   };
// private:
//   std::string id;
//   int ppqn;
//   int length;
//   int active_step;
//   bool unsaved;
//   sequence_t sequence;
//   playback_t playback;

//   void load_from_file(std::string filepath) {
//     // internally load
//   }
// };

// class Instrument {
// public:
// private:
//   std::string id;
  
//   std::vector<Part> parts;
//   int current_part_idx;
//   int sequence_state;
// };

// #endif
