#include "anemone/types/instrument/instrument.hpp"


Instrument::Instrument(InstrumentName name,
                       std::shared_ptr<Config> config,
                       std::shared_ptr<::Layout> layout,
                       std::vector<std::shared_ptr<Part> > parts,
                       sequence_layer_t default_midi_notes)
  : name(name),
    layout(layout),
    status({ .part = { .in_playback = rx::behavior<std::shared_ptr<Part> >(parts[0]),
                       .under_edit  = rx::behavior<std::shared_ptr<Part> >(parts[0])},
             .bank = { .in_playback = rx::behavior<bank_idx_t>(0),
                       .under_edit  = rx::behavior<bank_idx_t>(0)},
             .is_playing            = rx::behavior<bool>(true),
             .stop_on_next_measure  = rx::behavior<bool>(false)
      }),
    parts(parts),
    last_midi_notes_played(rx::behavior<sequence_layer_t>(default_midi_notes))
{}

void Instrument::update_last_midi_notes_played(midi_event_t midi_event) {
  // TODO add timing logic to aggregate closely timed updates!

  // convert midi_event into a step_event
  step_event_t step_event(step_event_protocol_t::Midi, midi_event.data);

  last_midi_notes_played.get_subscriber().on_next(sequence_layer_t{ {step_event.id, step_event} });
}

// this is a static method on Instrument, i.e. Instrument::create()
Instrument Instrument::create(InstrumentName name, std::shared_ptr<Config> config) {
  std::vector<std::shared_ptr<Part> > parts;

  // populate new parts TODO make this connfigurable....
  for (unsigned int i = 0; i < 49; ++i) {
    parts.push_back(std::make_shared<Part>(i));
  }

  // TODO parse instrument layout
  auto layout = std::make_shared<Instrument::Layout>(name, config);
  
  // TODO make default midi notes configurable
  auto default_note = step_event_t::make_midi_note_on("c4", 1, 127);
  sequence_layer_t default_notes = { {default_note.id, default_note } };
  
  return Instrument(name, config, layout, parts, default_notes);
}

Instrument::Layout::Layout(InstrumentName name, std::shared_ptr<Config> config)
  : ::Layout(name),
    config(config)
{}

void Instrument::Layout::register_sections() {
    // we want to parse grid regions for all possible instrument sub-components:
  // (1) pads (if they exist)
  // (2) pad parameters (if they exist)
  // (3) parameters (if they exist)
  // (4) keyboard (if it exists) TODO
  // (5) etc...

  // parse pad layouts
  if (config->yml["pads"]) {    
    for (auto itr : config->yml["pads"]) {
      // get the pad name
      auto pad_name = itr.first.as<std::string>();
      auto pad = Config(itr.second, config->path); // OKAY this is weird....we need to clean up this iterator slop.
      
      // parse grid section and add to sections
      auto grid_section = std::make_shared<GridSection>(name + pad_name, pad.parse_grid_region("layout"));
      register_section(grid_section);

      // now parse any parameters for this pad
      if (pad.yml["parameters"]) {

        for (auto param_itr : pad.yml["parameters"]) {

          // get the parameter name
          auto param_name = param_itr.first.as<std::string>();
          auto param = Config(itr.second, config->path); // OKAY this is weird....we need to clean up this iterator slop.

          // parse grid section and add to sections
          auto grid_section = std::make_shared<GridSection>(name + pad_name + param_name, param.parse_grid_region("layout"));
          register_section(grid_section);

        }
      }
    }
  }

  // parse param layouts
  if (config->yml["parameters"]) {    
    for (auto itr : config->yml["parameters"]) {

      // get the parameter name
      auto param_name = itr.first.as<std::string>();
      auto param = Config(itr.second, config->path); // OKAY this is weird....we need to clean up this iterator slop.

      // // parse grid section and add to sections
      auto grid_section_name = name + param_name;
      auto grid_region = param.parse_grid_region("layout");
      auto grid_section = std::make_shared<GridSection>(grid_section_name, grid_region);
      
      register_section(grid_section);
    }
  }
}
