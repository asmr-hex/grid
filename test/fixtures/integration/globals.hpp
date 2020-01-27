/**
 * @file   test/fixtures/integration/globals.hpp
 * @brief  Global Variables for Integration Tests
 * @author coco
 * @date   2020-01-26
 *************************************************/


#ifndef TEST_FIXTURES_INTEGRATION_GLOBALS_H
#define TEST_FIXTURES_INTEGRATION_GLOBALS_H

#include <map>
#include <string>
#include <memory>

#include "doubles/device/grid.hpp"
#include "doubles/device/midi.hpp"
#include "anemone/anemone.hpp"


/// @brief a global test grid device for integration tests
extern std::shared_ptr<BrowserGridDevice> test_grid_device;

/// @brief a global map of test midi input devices for integration tests
extern std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > test_midi_in_devices;

/// @brief a global map of test midi output devices for integration tests
extern std::shared_ptr<std::map<std::string, std::shared_ptr<BrowserMidiDevice> > > test_midi_out_devices;

/// @brief a global test anemone for integration tests
extern std::shared_ptr<Anemone> test_anemone;


#endif
