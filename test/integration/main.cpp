#define CATCH_CONFIG_RUNNER

#include <iostream>

#include <memory>
#include <thread>

#include <catch.hpp>
#include <spdlog/spdlog.h>

#include "anemone/anemone.hpp"
#include "anemone/io/midi/device/rtmidi.hpp"
#include "anemone/util/concurrent_queue.hpp"
#include "anemone/util/wait.hpp"
// #include "anemone/io.hpp"

#include "fixtures/integration/globals.hpp"


// define global integration test variables
std::shared_ptr<BrowserGridDevice> test_grid_device;
// TODO (coco|1.24.2020) eventually introduce test midi devices
std::shared_ptr<Anemone> test_anemone;


int main( int argc, char* argv[] ) {
  std::shared_ptr< Queue<bool> > browser_grid_ready;
  std::shared_ptr< Queue<bool> > anemone_ready;

  browser_grid_ready = std::make_unique<Queue<bool> >();
  anemone_ready = std::make_unique<Queue<bool> >();

  // parse the mode test mode we are using
  std::string mode_arg(argv[6]);
  BrowserGridDevice::Mode test_mode;
  if ( mode_arg == "headless" ) {
    test_mode = BrowserGridDevice::Mode::Headless;
  } else if (mode_arg == "visual" ) {
    test_mode = BrowserGridDevice::Mode::Visual;
  } else if (mode_arg == "interactive" ) {
    test_mode = BrowserGridDevice::Mode::Interactive;
  } else {
    spdlog::error("Invalid integration test mode '{}' provided!", mode_arg);
    spdlog::error("use: 'headless', 'visual', or 'interactive'");

    return -1;
  }

  // parse the wait duration after each toggle (but only if we are in visual/interactive mode)
  unsigned int wait_duration = 0;
  if (test_mode == BrowserGridDevice::Mode::Visual || test_mode == BrowserGridDevice::Mode::Interactive) {
    std::string wait_arg(argv[7]);
    wait_duration = std::stoi(wait_arg);
  }
  
  // create test grid device
  test_grid_device = std::make_shared<BrowserGridDevice>(test_mode, wait_duration, browser_grid_ready);

  
  // create global anemone object
  test_anemone = std::make_shared<Anemone>(Anemone(argv[5],
                                                   test_grid_device,
                                                   std::make_shared<RTMidi>(),
                                                   anemone_ready)
                                           );

  // embed grid layout into test grid device s.t. our tests can be layout agnostic!
  test_grid_device->set_layout_context(&test_anemone->io->grid->layout);
  
  // start test anemone in its own thread
  std::thread t([] { test_anemone->run(); });
  t.detach();

  
  // wait for anemone thread to have already started
  wait_for(2000);

  if (test_mode != BrowserGridDevice::Mode::Headless) {
    if (test_mode == BrowserGridDevice::Mode::Visual)
      spdlog::warn("you have entered visual integration test mode!");
    if (test_mode == BrowserGridDevice::Mode::Interactive)
      spdlog::warn("you have entered interactive integration test mode!");

    spdlog::warn("please navigate in your web browser to:");
    spdlog::warn("http://localhost:{}", test_anemone->config->ports.grid);

    // wait for browser grid to be ready
    browser_grid_ready->pop();
  }
  
  // wait for anemone to be ready
  anemone_ready->pop();

  // print address of global
  std::cout << test_grid_device <<std::endl;
  
  // only pass catch relevant (the first 5 arguments -- see makefile)
  int new_argc = 5;
  int result = Catch::Session().run( new_argc, argv );

  // global clean-up...
  if (test_mode != BrowserGridDevice::Mode::Headless) {
    // TODO report success/failure in browser
  }

  return result;
}
