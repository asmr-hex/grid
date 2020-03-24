/**
 * @file   types/io/grid/layout/layout.hpp
 * @brief  IO Grid Layout Base Class Type
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef TYPES_IO_GRID_LAYOUT_H
#define TYPES_IO_GRID_LAYOUT_H

#include <map>
#include <tuple>
#include <vector>
#include <memory>
#include <functional>

#include "anemone/types/io/grid/event.hpp"
#include "anemone/types/io/grid/address.hpp"
#include "anemone/types/io/grid/device/event.hpp"
#include "anemone/types/io/grid/device/coordinates.hpp"
#include "anemone/types/io/grid/layout/names.hpp"
#include "anemone/types/io/grid/layout/section.hpp"


// forward declare state to avoid circular dependencies.
class State;


/// @brief Base class for grid layouts.
///
/// @details to implement this base class, one must implement the `register_sections`
/// method in which we register all sections and sublayouts. since `shared_from_this()`
/// is called from within the register_sublayout method, this must not be called within
/// the constructor which is why it is broken out into a register sections method.
/// finally, this virtual method (once implemented) is invoked upon `connect()`.
/// 
class Layout : public std::enable_shared_from_this<Layout> {
public:
  /// @brief the name of the layout
  LayoutName name;

  Layout(LayoutName name);
  virtual ~Layout() = default;

  /// @brief Translates an inbound `grid_device_event_t` to a more specific `grid_event_t`.
  ///
  /// @param event   a `grid_device_event_t` originating from the grid device.
  ///
  /// @ return a high level `grid_event_t`.
  ///
  grid_event_t translate(const grid_device_event_t& event) const;

  /// @brief Translates a high-level `grid_addr_t` to a low-level `grid_coordinates_t`.
  ///
  /// @param address   a high-level `grid_addr_t` address
  ///
  /// @return a low-level `grid_coordinates_t` coordinate
  ///
  grid_coordinates_t translate(const grid_addr_t&) const;
  
  /// @brief subscribes to state changes which cause layout changes and calls `register_sections`.
  ///
  /// @details executes all registered sublayout handler functions and calls this
  /// method on each of the sublayouts registered to this layout.
  /// the idea behind this method is as follows. since a layout can have multiple
  /// sublayouts registered to one section (e.g. sequencer.instrument_panel has all
  /// the instrument sublayouts registered to it), a parent layout needs to be able
  /// to update which is the current sublayout active for a given section. to do this,
  /// we will inject the full state into each layout (via this method) to allow derived
  /// classes to subscribe to whatever part of the state tree is necessary to signal an
  /// update to the sublayout.
  ///
  /// @remark this method should be invoked for each top-level layout after the state has been
  /// fully initialized (for example at the very end of the state root constructor).
  ///
  /// @param state   a pointer to the fully initialized state.
  ///
  void connect(std::shared_ptr<State>);
  
  /// @brief a map of sections by name.
  std::map< GridSectionName, std::shared_ptr<GridSection> > section_by_name;

  /// @brief a vector of all registered sublayouts.
  ///
  /// @details it is necessary to keep track of all registered sublayouts so that
  /// when `connect` is called, we can also call it for each sublayout.
  std::vector< std::shared_ptr<Layout> > sublayouts;
  
protected:
  /// @brief virtual method for registering sections and sublayouts.
  ///
  /// @remark this method is where all registration should occur. this is
  /// important because we can't call registration methods within the Layout
  /// constructor since they make use of `shared_from_this()`.
  ///
  virtual void register_sections() = 0;

  void register_section(std::shared_ptr<GridSection>);

  /// @brief registers a sublayout to be associated with a specific section of this
  /// current layout.
  ///
  /// @details registering sublayouts is important for translating grid addresses
  /// to grid coordinates and vice versa. since layouts support sublayouts, we need
  /// to keep track of a given layouts parent and child layouts so that when we are
  /// performing these translations, they know how to translate from a parent layout
  /// address to a child layout address and vice versa.
  ///
  /// @param section_name   the grid section name of this layout we wish to register a sublayout with.
  /// @param child_layout   a pointer to the sublayout we wish to register.
  ///
  void register_sublayout(GridSectionName, std::shared_ptr<Layout>);

  /// @brief adds a sublayout handler which will be executed when `connect`
  /// is invoked.
  ///
  /// @param handler_function   a handler function which accepts a pointer to the fully initialized state.
  void add_sublayout_update_handler(std::function<void(std::shared_ptr<State>)>);

  /// @brief updates the current sublayout for a given grid section in this layout.
  ///
  /// @remark this is usually called from within a handler function added via the
  /// `add_sublayout_update_handler` method. Additionally, the sublayout provided
  /// is assumed to have already been registered.
  ///
  /// @param section_name   the grid section name of this layout we wish to update.
  /// @param child_layout   a pointer to the sublayout we wish to use.
  ///
  void update_sublayout_for(GridSectionName, std::shared_ptr<Layout>);
  
private:
  /// @brief registerd sections of this layout.
  std::vector<std::shared_ptr<GridSection> > sections;

  /// @brief a vector of handler functions for sublayout updates.
  ///
  /// @details all these handlers are executed when `connect` is called.
  std::vector< std::function<void(std::shared_ptr<State>)> > sublayout_update_handlers;
  
  /// @brief current sublayouts per section. since a grid section can only have one
  /// current sublayout at a given time, these entries are updated according to the
  /// logic defined in the implementation of `connect`.
  std::map<GridSectionName, std::shared_ptr<Layout> > current_sublayout_by_section;

  /// @brief registered superlayout. this never changes.
  struct {
    GridSectionName section;
    std::shared_ptr<Layout> layout = nullptr;  
  } superlayout;

  /// @brief this is a flat map of all nested layouts. this is important because calls
  /// to the translate method are sometimes for grid addresses of sublayouts to the current
  /// rendered top-level layout. keeping this map ensures that we don't have to search for the
  /// appropriate sublayout on each translate.
  std::map<LayoutName, std::shared_ptr<Layout> > sublayout_flat_map_by_name;
  
  /// @brief returns the grid layout, section, and coordinates corresponding to the provided grid coordinates.
  ///
  /// @details this method will convert global grid coordinates to the coordinates of the most specific section.
  ///
  /// @param grid_coordinates                    `grid_coordinates_t` to fidn the section of.
  ///
  /// @return layout/section/coordinates tuple   a tuple of ptr to `Layout`, ptr to a `GridSection`, and coordinates.
  ///
  std::tuple<LayoutName, std::shared_ptr<GridSection>, grid_coordinates_t>
  layout_section_and_coords_of(const grid_coordinates_t&) const;

  /// @brief registers a super (parent) layout for the current layout.
  ///
  /// @details this is invoked only within the `register_sublayout` method and
  /// registers the calling layout (the parent) to the layout it is adding as a
  /// sublayout.
  ///
  /// @param section_name    the grid section name of the parent layout for which this is a sublayout.
  /// @param parent_layout   a pointer to the parent layout we are registering.
  ///
  void register_superlayout(GridSectionName, std::shared_ptr<Layout>);

  // allow other layouts to access private methods of layouts. this is important specifically
  // to allow a layout to call `register_superlayout` on another layout from within the
  // `register_sublayout` method.
  friend class Layout;
};

#endif
