#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <map>
#include <vector>
#include <string>

#include <monome.h>

#include "../sequencer/part.hpp"
#include "../config/config.hpp"
#include "../io/io.hpp"


class Instrument {
public:
  Instrument(Config *config, IO *io) : config(config), io(io) {
    current_part = 0;
    current_bank = 0;
    // TODO initialize better
    parts.push_back(new Part(0, config, io));
  };
  
  Part *get_current_part() {
    return parts[current_part];
  };

  void play_part(int part_idx) {
    // TODO implement me!
  };

  void edit_part(int part_idx) {
    if (part_idx == part.under_edit) return;

    mapping_coordinates_t old_under_edit_coords = config->mappings.parts.get_coordinates_from_sequential_index(part.under_edit);
    mapping_coordinates_t new_under_edit_coords = config->mappings.parts.get_coordinates_from_sequential_index(part_idx);
    
    // turn off or turn down current under edit part led
    if (part.under_edit == part.in_playback) {
      // set this led to 'in playback' brightness
      monome_led_level_set(io->output.monome,
                           old_under_edit_coords.x,
                           old_under_edit_coords.y,
                           led_brightness.part.in_playback);
    } else {
      // turn off the currently under edit part led
      monome_led_level_set(io->output.monome,
                           old_under_edit_coords.x,
                           old_under_edit_coords.y,
                           led_brightness.part.off);
    }

    // update part under edit
    part.under_edit = part_idx;
    
    // set this led to 'under_edit' brightness
    monome_led_level_set(io->output.monome,
                         new_under_edit_coords.x,
                         new_under_edit_coords.y,
                         led_brightness.part.under_edit);

    // TODO render new part under edit
  };
  
  int current_part;
  int current_bank;
  bool is_playing;
  bool stop_on_next_measure;
  
protected:
  std::string name;
  Config *config;
  IO *io;

  std::vector<Part*> parts;
  
  struct {
    int in_playback = 0;
    int under_edit = 0;
  } part;

  struct {
    struct {
      int in_playback = 4;
      int under_edit = 15;
      int off = 0;
    } part;

    struct {
      int in_playback = 0;
      int under_edit = 15;
      int off = 4;
    } bank;
    
  } led_brightness;
  
};

#endif
