#include "utilities/recorder.hpp"


TestOutputRecorder::TestOutputRecorder(std::shared_ptr<BrowserGridDevice> grid_device,
                                       std::shared_ptr<Anemone> anemone)
  : grid_device(grid_device), anemone(anemone) {
  // extract midi devices from anemone..? or wait...this must have already happened if we are going to inject stuff into these babies!
  
}
