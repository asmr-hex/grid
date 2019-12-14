#ifndef TEST_FIXTURES_LAYOUTS_H
#define TEST_FIXTURES_LAYOUTS_H

#include "optional"

#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/section.hpp"
#include "anemone/io/grid/layout/names.hpp"


// derive a small test sequencer layout
class TestSequencerLayout : public Layout {
public:
  GridSection steps;
  GridSection play_pause;
  GridSection switch_layout;
  
  TestSequencerLayout() : Layout(LayoutName::SequencerAndInstrument) {
    steps = GridSection(GridSectionName::Steps,
                        { .min = { .x = 0, .y = 0},
                          .max = { .x = 15, .y = 1} });
    play_pause = GridSection(GridSectionName::PlayPause,
                             { .min = { .x = 0, .y = 2},
                               .max = { .x = 0, .y = 2} });
    switch_layout = GridSection(GridSectionName::SwitchLayout,
                                { .min = { .x = 15, .y = 7},
                                  .max = { .x = 15, .y = 7} });
    
    register_section(steps);
    register_section(play_pause);
    register_section(switch_layout);
  };

protected:
  virtual std::optional<LayoutName>  change_layout(const grid_event_t& event) const override {
    if (event.section == GridSectionName::SwitchLayout) {
      return LayoutName::ParallelSequencer;
    }

    return std::nullopt;
  };
};

class TestParallelSequencerLayout : public Layout {
public:
  GridSection steps;
  GridSection play_pause;
  GridSection switch_layout;

  TestParallelSequencerLayout() : Layout(LayoutName::ParallelSequencer) {
    steps = GridSection(GridSectionName::Steps,
                        { .min = { .x = 0, .y = 0},
                          .max = { .x = 15, .y = 6} });
    play_pause = GridSection(GridSectionName::PlayPause,
                             { .min = { .x = 0, .y = 2},
                               .max = { .x = 0, .y = 2} });
    switch_layout = GridSection(GridSectionName::SwitchLayout,
                                { .min = { .x = 15, .y = 7},
                                  .max = { .x = 15, .y = 7} });

    register_section(steps);
    register_section(play_pause);
    register_section(switch_layout);
  };

protected:
  virtual std::optional<LayoutName>  change_layout(const grid_event_t& event) const override {
    if (event.section == GridSectionName::SwitchLayout) {
      return LayoutName::SequencerAndInstrument;
    }

    return std::nullopt;
  };
};


#endif
