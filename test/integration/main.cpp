#define CATCH_CONFIG_RUNNER

#include <memory>
#include <thread>
#include <chrono>

#include <catch.hpp>
#include <spdlog/spdlog.h>

#include "anemone/anemone.hpp"
#include "anemone/io/midi/device/rtmidi.hpp"
#include "anemone/util/concurrent_queue.hpp"
// #include "anemone/io.hpp"

#include "doubles/device/grid.hpp"


std::shared_ptr<BrowserGridDevice> test_grid_device;
// TODO (coco|1.24.2020) eventually introduce test midi devices
std::shared_ptr<Anemone> test_anemone;

int main( int argc, char* argv[] ) {
  std::shared_ptr< Queue<bool> > browser_grid_ready;
  std::shared_ptr< Queue<bool> > anemone_ready;

  browser_grid_ready = std::make_unique<Queue<bool> >();
  anemone_ready = std::make_unique<Queue<bool> >();

  std::string interactive_arg(argv[6]);
  bool interactive = false;
  if ( interactive_arg == "true" )
    interactive = true;
  
  // create test grid device
  test_grid_device = std::make_shared<BrowserGridDevice>(interactive, browser_grid_ready);

  // create global anemone object
  test_anemone = std::make_shared<Anemone>(Anemone(argv[5],
                                                   test_grid_device,
                                                   std::make_shared<RTMidi>(),
                                                   anemone_ready)
                                           );


  
  // start test anemone in its own thread
  std::thread t([] { test_anemone->run(); });
  t.detach();

  // wait for anemone thread to have already started
  std::this_thread::sleep_for(std::chrono::seconds(2));
  
  if (interactive) {
    spdlog::warn("you have entered interactive integration test mode!");
    spdlog::warn("please navigate in your web browser to:");
    spdlog::warn("http://localhost:{}", test_anemone->config->ports.grid);
  }

  
  // wait for browser grid to be ready
  browser_grid_ready->pop();

  // wait for anemone to be ready
  anemone_ready->pop();

  // only pass catch relevant (the first 5 arguments -- see makefile)
  int new_argc = 5;
  int result = Catch::Session().run( new_argc, argv );

  // global clean-up...
  if (interactive) {
    // TODO report success/failure in browser
  }

  return result;
}
