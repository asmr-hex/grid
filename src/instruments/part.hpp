#ifndef PART_H
#define PART_H

#include <map>
#include <string>
#include <vector>

#include "../types.hpp"
#include "../consts.hpp"


class Part {
public:
  Part(int id) : id(id) {
    // load part file if it exists.
    active_step = -1; // we want to initialize active_step to -1 whenever we are starting the sequence from the beginning

    // TODO remove this test
    ppqn = 1;
    length = 2;
    step_event_t se1 = { OSC, 0x0000, std::vector<unsigned char>(0x00FF) };
    step_event_t se2 = { OSC, 0x0100, std::vector<unsigned char>(0x00FF) };
    std::map<event_uid_t, step_event_t> m1;
    m1[0x0000] = se1;
    std::map<event_uid_t, step_event_t> m2;
    m2[0x0001] = se2;
    sequence[0] = m1;
    sequence[1*constants::PPQN] = m2;
  };

  void load(int new_part_idx) {
    
  };

  void save() {
    
  };
  
  std::vector<step_event_t> advance() {
    std::vector<step_event_t> next_events;

    // if playback is not playing, do not advance.
    
    // how far do we skip ahead for the next active step?
    // int step_size = constants::PPQN / ppqn;

    // if ppqn = 1, we want step_size = 1
    // if ppqn = 2, we want ste_size = 2;
    // etc.
    
    // advance to next step
    active_step += ppqn;

    // std::cout << "ACTIVE STEP " << active_step << std::endl;
    
    // if the active step is now greater than the last step, circle back
    if (active_step > (length * constants::PPQN) - 1) {
      // std::cout << "RESTARTING LOOP\n";
      active_step = 0;
    }

    try {
      // get events from new active step
      auto step = sequence.at(active_step);

      // std::cout << "GETTING HERE\n";
      for (auto i : step) {
        // copy sequence event into results
        next_events.push_back(i.second);
      }
    } catch (std::out_of_range &error) {
      // this step is empty. carry on.
      // std::cout << "lets see this error: " << error.what() << std::endl;;
    }

    return next_events;
  };
  
private:
  int id;
  int ppqn;
  int length;
  int active_step;
  bool unsaved;
  sequence_t sequence;
  playback_t playback;

  void load_from_file(std::string filepath) {
    // internally load
  }
};


#endif
