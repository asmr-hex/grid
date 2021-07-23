/**
 * @file   plugins/instruments/instrument_plugins.hpp
 * @brief  Instrument Plugin Manager Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_INSTRUMENT_PLUGIN_MANAGER_H
#define PLUGINS_INSTRUMENT_PLUGIN_MANAGER_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/plugins/plugin.hpp"

#include "anemone/ui/component.hpp"
#include "anemone/ui/layout_ui.hpp"
#include "anemone/controllers/controller.hpp"


// include all specific instrument plugins
// #include "anemone/plugins/instruments/er1/er1.hpp"
// #include "anemone/plugins/instruments/microgranny/microgranny.hpp"


// TODO eventually call this Instrument Component, since it will be an instrument component
// for a specific instrument
// NOTES: in the future, a "Component" will not be derived from an instrument, but rather take in the
// instrument as a parameter.
class InstrumentPlugin: public Plugin, public Instrument, public std::enable_shared_from_this<InstrumentPlugin> {
public:
  InstrumentPlugin(InstrumentName, std::shared_ptr<Config>);

  virtual std::shared_ptr<::Layout>
  get_layout() override;

  virtual std::shared_ptr<StateController>
  make_controller(std::shared_ptr<IO>, std::shared_ptr<State>) override;

  virtual std::shared_ptr<LayoutUI>
  make_ui(std::shared_ptr<IO>, std::shared_ptr<State>) override;

  /// @brief nested Controller class for Instrument components
  class Controller : public StateController {
  public:
    /// @brief nested Block for instrument controller building block
    class Block {
    public:
      /// @brief controller block type
      enum class Type { Pad, Parameter, Keyboard };

      // TODO create a controller block config

      Block(std::shared_ptr<IO>,
            std::shared_ptr<State>,
            std::shared_ptr<Instrument>);
    };
    
    Controller(std::shared_ptr<Config>,
               std::shared_ptr<IO>,
               std::shared_ptr<State>,
               std::shared_ptr<Instrument>);

    /// @brief vector of controller blocks
    std::vector< std::unique_ptr<Block> > blocks;
    
  };

  /// @brief nested UI class for Instrument components
  class UI : public LayoutUI {
  public:
    /// @brief nested Block for instrument UI building block
    class Block : public UIComponent {
    public:
      /// @brief ui block type
      enum class Type { Pad, Parameter, Keyboard };

      // TODO create a ui block config
      
      Block(LayoutName,
            GridSectionName,
            std::shared_ptr<IO>,
            std::shared_ptr<State>,
            std::shared_ptr<Instrument>);
    };
    
    UI(std::shared_ptr<Config>,
       std::shared_ptr<IO>,
       std::shared_ptr<State>,
       std::shared_ptr<Instrument>);

    /// @brief vector of ui blocks
    std::vector< std::unique_ptr<Block> > blocks;
    
  };
};


class InstrumentPluginManager {
public:
  InstrumentPluginManager(std::shared_ptr<Config>);

  std::shared_ptr<Instrument>               default_instrument;
  std::vector<std::shared_ptr<Instrument> > instruments;
  std::vector<std::shared_ptr<Plugin> >     plugins;
};

#endif
