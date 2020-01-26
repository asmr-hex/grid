/**
 * @file   test/fixtures/integration/globals.hpp
 * @brief  Global Variables for Integration Tests
 * @author coco
 * @date   2020-01-26
 *************************************************/


#ifndef TEST_FIXTURES_INTEGRATION_GLOBALS_H
#define TEST_FIXTURES_INTEGRATION_GLOBALS_H

#include <memory>

#include "doubles/device/grid.hpp"
#include "anemone/anemone.hpp"

/// @brief a global test grid device for integration tests
extern std::shared_ptr<BrowserGridDevice> test_grid_device;

// TODO (coco|1.24.2020) eventually introduce test midi devices

/// @brief a global test anemone for integration tests
extern std::shared_ptr<Anemone> test_anemone;


#endif
