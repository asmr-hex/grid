#ifndef TEST_FIXTURES_LAYOUTS_H
#define TEST_FIXTURES_LAYOUTS_H

#include <memory>
#include "optional"

#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/section.hpp"
#include "anemone/io/grid/layout/names.hpp"


// derive a small test sequencer layout
class TestSequencerLayout : public Layout {
public:
  std::shared_ptr<GridSection> steps;
  std::shared_ptr<GridSection> play_pause;
  std::shared_ptr<GridSection> switch_layout;
  
  TestSequencerLayout() : Layout(LayoutName::SequencerAndInstrument) {
    steps = std::make_shared<GridSection>
      (GridSection(GridSectionName::Steps,
                   { .min = { .x = 0, .y = 0},
                     .max = { .x = 15, .y = 1} }
                   ));
    play_pause = std::make_shared<GridSection>
      (GridSection(GridSectionName::PlayPause,
                   { .min = { .x = 0, .y = 2},
                     .max = { .x = 0, .y = 2} }
                   ));
    switch_layout = std::make_shared<GridSection>
      (GridSection(GridSectionName::SwitchLayout,
                   { .min = { .x = 15, .y = 7},
                     .max = { .x = 15, .y = 7} }
                   ));
    
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
  std::shared_ptr<GridSection> steps;
  std::shared_ptr<GridSection> play_pause;
  std::shared_ptr<GridSection> switch_layout;

  TestParallelSequencerLayout() : Layout(LayoutName::ParallelSequencer) {
    steps = std::make_shared<GridSection>
      (GridSection(GridSectionName::Steps,
                   { .min = { .x = 0, .y = 0},
                     .max = { .x = 15, .y = 6} }
                   ));
    play_pause = std::make_shared<GridSection>
      (GridSection(GridSectionName::PlayPause,
                   { .min = { .x = 0, .y = 2},
                     .max = { .x = 0, .y = 2} }
                   ));
    switch_layout = std::make_shared<GridSection>
      (GridSection(GridSectionName::SwitchLayout,
                   { .min = { .x = 15, .y = 7},
                     .max = { .x = 15, .y = 7} }
                   ));

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
