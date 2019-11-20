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
    parts[0][0] = new Part(0, config, io);
  };

  Part *get_part_in_playback() {
    return parts[bank.in_playback][part.in_playback];
  }

  Part *get_part_under_edit() {
    return parts[bank.under_edit][part.under_edit];
  }

  void render() {
    render_part();
    
    // render part / bank selection panels
    render_part_selection_panel();
    render_bank_selection_panel();
  };
  
  // renders the part under edit by default in the ui (monome grid).
  void render_part() {
    render_part(bank.under_edit, part.under_edit);
  };
  
  // renders a part of the ui (monome grid).
  void render_part(int bank_idx, int part_idx) {
    ensure_part(bank_idx, part_idx);
    
    bool force_rerender = true;
    Part *part_to_render = parts[bank_idx][part_idx];

    // TODO have a method on Part called "render" which does all this stuff...
    // render part and its sub-components
    part_to_render->render_page(part_to_render->page.rendered, force_rerender);
    part_to_render->render_page_selection_ui();
    part_to_render->render_ppqn_selection_ui();
    // TODO render part's playback status
  };

  // renders the part selection ui panel.
  //
  // notably, this method clears the entire panel and thus, for optimization purposes, should
  // only be used when initially rendering this sub-section of the ui for a given instrument
  // and *not* for incrementally switching parts.
  void render_part_selection_panel() {
    // set entire part selection panel to 'off'
    set_led_region_intensity(io, &config->mappings.parts, led_brightness.part.off);

    // get coordinates of parts under edit / in playback
    mapping_coordinates_t part_under_edit = config->mappings.parts.get_coordinates_from_sequential_index(part.under_edit);
    mapping_coordinates_t part_in_playback = config->mappings.parts.get_coordinates_from_sequential_index(part.in_playback);

    // set leds for parts under edit / in playback
    monome_led_level_set(io->output.monome, part_in_playback.x, part_in_playback.y, led_brightness.part.in_playback);
    monome_led_level_set(io->output.monome, part_under_edit.x, part_under_edit.y, led_brightness.part.under_edit);
  };

  // renders the bank selection ui panel.
  //
  // notably, this method clears the entire panel and thus, for optimization purposes, should
  // only be used when initially rendering this sub-section of the ui for a given instrument
  // and *not* for incrementally switching banks.
  void render_bank_selection_panel() {
    // set entire part selection panel to 'off'
    set_led_region_intensity(io, &config->mappings.banks, led_brightness.bank.off);

    // get coordinates of banks under edit / in playback
    mapping_coordinates_t bank_under_edit = config->mappings.banks.get_coordinates_from_sequential_index(bank.under_edit);
    mapping_coordinates_t bank_in_playback = config->mappings.banks.get_coordinates_from_sequential_index(bank.in_playback);

    // set leds for banks under edit / in playback
    monome_led_level_set(io->output.monome, bank_in_playback.x, bank_in_playback.y, led_brightness.bank.in_playback);
    monome_led_level_set(io->output.monome, bank_under_edit.x, bank_under_edit.y, led_brightness.bank.under_edit);
  };

  // toggle the playback state (play/pause) of the current part under edit.
  //
  // if the current part under edit is not the part in playback, they part in
  // playback is scheduled to stop on the completion of its current sequence
  // cycle and hand over playback to the part under edit.
  void play_or_pause_part() {
    Part *part_under_edit = get_part_under_edit();
    Part *part_in_playback = get_part_in_playback();
    
    if (part.under_edit == part.in_playback) {
      // when the part under edit *is* the part in playback, we simply
      // play or pause the part depending on the part's playback state
      
      if (part_in_playback->playback.is_playing) {
        // current part is in playback and is playing, pause it
        part_in_playback->pause_playback();
      } else {
        // current part is in playback and is paused or stopped, play it
        part_in_playback->begin_playback();
      }
      
    } else {
      // current part is under edit and not in playback.
      
      // schedule current in playback part to stop on next cycle
      // and hand over playback to part under edit.
      part_in_playback->enqueue_next_part_for_playback(part_under_edit);
      
      // TODO there might need to be some state in between the hand-off
      // from a playing part to a not playing part... maybe indicated by
      // a slowly blinking led on the part...?
    }

    // update playback led for current part
    int play_pause_led_brightness;
    if (part_under_edit->playback.is_playing) {
      play_pause_led_brightness = led_brightness.playback.play;
    } else if (part_under_edit->playback.is_paused) {
      play_pause_led_brightness = led_brightness.playback.pause;
    } else if (!part_under_edit->playback.is_playing) {
      play_pause_led_brightness = led_brightness.playback.stop;
    }
    monome_led_level_set(io->output.monome,
                         config->mappings.play_pause.x,
                         config->mappings.play_pause.y,
                         play_pause_led_brightness);
  };

  void stop_part() {
    // stop part in playback!
    get_part_in_playback()->stop_playback();

    if (part.under_edit == part.in_playback) {
      // update the start/pause led to be off
      monome_led_level_set(io->output.monome,
                           config->mappings.play_pause.x,
                           config->mappings.play_pause.y,
                           led_brightness.playback.stop);
    }
  };
  
  // selects a new part to edit and renders it in the ui. this variant method uses the currently
  // 'under edit' bank since it assumes that only a part was selected without a bank.
  void edit_part(int part_idx) {
    edit_part(bank.under_edit, part_idx);
  };
  
  // selects a new part to edit and renders it in the ui (monome grid)
  void edit_part(int bank_idx, int part_idx) {
    if (bank_idx == bank.under_edit && part_idx == part.under_edit) return;

    // okay...either the bank or the part under edit changed.
    
    mapping_coordinates_t old_under_edit_coords;
    mapping_coordinates_t new_under_edit_coords;
    
    if (part_idx != part.under_edit) {
      // the part unnder edit has changed!
 
      old_under_edit_coords = config->mappings.parts.get_coordinates_from_sequential_index(part.under_edit);
      new_under_edit_coords = config->mappings.parts.get_coordinates_from_sequential_index(part_idx);
      
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
 
      // set this led to 'under_edit' brightness
      monome_led_level_set(io->output.monome,
                           new_under_edit_coords.x,
                           new_under_edit_coords.y,
                           led_brightness.part.under_edit);
      
    }

    if (bank_idx != bank.under_edit) {
      // the bank under edit has changed!

      old_under_edit_coords = config->mappings.banks.get_coordinates_from_sequential_index(bank.under_edit);
      new_under_edit_coords = config->mappings.banks.get_coordinates_from_sequential_index(bank_idx);
      
      // turn off or turn down current under edit part led
      if (bank.under_edit == bank.in_playback) {
        // set this led to 'in playback' brightness
        monome_led_level_set(io->output.monome,
                             old_under_edit_coords.x,
                             old_under_edit_coords.y,
                             led_brightness.bank.in_playback);
      } else {
        // turn off the currently under edit part led
        monome_led_level_set(io->output.monome,
                             old_under_edit_coords.x,
                             old_under_edit_coords.y,
                             led_brightness.bank.off);
      }
      
      // set this led to 'under_edit' brightness
      monome_led_level_set(io->output.monome,
                           new_under_edit_coords.x,
                           new_under_edit_coords.y,
                           led_brightness.part.under_edit);
    }

    // ensure part exists
    ensure_part(bank_idx, part_idx);
      
    // update bank / part under edit
    part.under_edit = part_idx;
    bank.under_edit = bank_idx;
      
    // render new part under edit
    render_part(bank_idx, part_idx);
  };
    
protected:
  std::string name;
  Config *config;
  IO *io;

  std::map<int, std::map<int, Part*> > parts;
  
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

    struct {
      int play = 15;
      int pause = 5;
      int stop = 0;
    } playback;
  } led_brightness;

  // checks if a part exists and creates a new empty one if it doesn't
  void ensure_part(int bank_idx, int part_idx) {
    try {
      // does part exist?
      parts.at(bank_idx).at(part_idx);
    } catch (std::out_of_range &error) {
      // the part doesn't exist. lets' create it!
      parts[bank_idx][part_idx] = new Part(part_idx, config, io);
    }
  }
};

#endif
