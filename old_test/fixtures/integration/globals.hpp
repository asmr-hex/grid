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
#include "utility/recorder.hpp"


/// @brief a global test grid device for integration tests
extern std::shared_ptr<BrowserGridDevice> test_grid_device;

/// @brief a global test midi manager for integration tests
extern std::shared_ptr<BrowserMidiDeviceFactory> test_midi_manager;

/// @brief a global test anemone for integration tests
extern std::shared_ptr<Anemone> test_anemone;

/// @brief a global test utility for integration tests
extern std::shared_ptr<TestOutputRecorder> test_utility;

#endif
