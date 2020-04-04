#ifndef IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H
#define IO_GRID_LAYOUT_LAYOUTS_SEQUENCER_H

#include <memory>

#include "anemone/config.hpp"

#include "anemone/types/io/grid/layout/names.hpp"
#include "anemone/types/io/grid/layout/layout.hpp"
#include "anemone/types/io/grid/layout/section.hpp"


// forward declare
class PluginManager;


namespace GridLayout {

  class Sequencer : public Layout {
  public:
    std::shared_ptr<PluginManager> plugin_manager;

    std::shared_ptr<GridSection> control_select;
    std::shared_ptr<GridSection> instrument_select;
    std::shared_ptr<GridSection> instrument_panel;
    std::shared_ptr<GridSection> instrument_controls;
    std::shared_ptr<GridSection> steps;
    std::shared_ptr<GridSection> pages;
    std::shared_ptr<GridSection> parts;
    std::shared_ptr<GridSection> banks;
    std::shared_ptr<GridSection> ppqn;
    std::shared_ptr<GridSection> shift;
    std::shared_ptr<GridSection> play_pause;
    std::shared_ptr<GridSection> stop;
    std::shared_ptr<GridSection> record;
    std::shared_ptr<GridSection> last_step;
    std::shared_ptr<GridSection> metronome;

    Sequencer(std::shared_ptr<Config>, std::shared_ptr<PluginManager>);

    virtual void register_sections() override;
  };
};

#endif
