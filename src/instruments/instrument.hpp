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
  bool is_playing;
  bool stop_on_next_measure;

  struct {
    int in_playback = 0;
    int under_edit = 0;
  } part;

  struct {
    int in_playback = 0;
    int under_edit = 0;
  } bank;

  
  Instrument(Config *config, IO *io) : config(config), io(io) {
    // TODO initialize better
    parts[0] = new Part(0, config, io);
  };

  Part *get_part_in_playback() {
    return parts[part.in_playback];
  }

  Part *get_part_under_edit() {
    return parts[part.under_edit];
  }
  
  void play_part(int part_idx) {
    // TODO implement me!
  };

  // selects a new part to edit and renders it in the ui (monome grid)
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

    // ensure part exists
    ensure_part(part_idx);
    
    // update part under edit
    part.under_edit = part_idx;
    
    // set this led to 'under_edit' brightness
    monome_led_level_set(io->output.monome,
                         new_under_edit_coords.x,
                         new_under_edit_coords.y,
                         led_brightness.part.under_edit);

    // render new part under edit
    render_part(part_idx);
  };
    
protected:
  std::string name;
  Config *config;
  IO *io;

  std::map<int, Part*> parts;
  
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

  // renders a part ot the ui (monome grid).
  void render_part(int part_idx) {
    bool force_rerender = true;
    parts[part_idx]->render_page(parts[part_idx]->page.rendered, force_rerender);
    parts[part_idx]->render_page_selection_ui();
  };

  // checks if a part exists and creates a new empty one if it doesn't
  void ensure_part(int part_idx) {
    try {
      // does part exist?
      parts.at(part_idx);
    } catch (std::out_of_range &error) {
      // the part doesn't exist. lets' create it!
      parts[part_idx] = new Part(part_idx, config, io);
    }
  }
};

#endif
