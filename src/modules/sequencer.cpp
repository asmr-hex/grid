#include <opr/modules>

#include <iostream>

using namespace opr::module;

// Sequencer::Sequencer()
//   : step({ .current = rxcpp::subjects::behavior<step_idx_t>(0) })
// {}

void Sequencer::connect() {
    std::printf("sequencer loadingsx\n");
}

void Sequencer::sync(std::string name, rx::observable<bool> clk) {
  // add new clock to clocks mapping
  clocks[name] = clk;

  // merge new clock observable
  if (clocks.size() == 1) {
    merged_clocks_observable = clk;
  } else {
    merged_clocks_observable = merged_clocks_observable.merge(clk);     
  }

  // subscribe
  subscribe_to_clocks();
}

void Sequencer::subscribe_to_clocks() {
  // unsubscribe
  clocks_subscription.unsubscribe();

  clocks_subscription = merged_clocks_observable
    .subscribe([this] (bool _) {
                 step.current(step.current() + step.stride.get_value());
               });
}

void Sequencer::unsync(std::string name) {
  // remove clock from mapping
  clocks.erase(name);

  if (clocks.size() == 0) return;
  
  // create new merged subscription
  rx::observable<bool> o;
  bool init = true;
  for (auto itr : clocks) {
    if (init) {
      o = itr.second;
      init = false;
    } else {
      o = o.merge(itr.second); 
    }
  }
  merged_clocks_observable = o;
  
  subscribe_to_clocks();
}


void Sequencer::Step::current(step_idx_t i) {
  current_m.get_subscriber().on_next(i);
}

step_idx_t Sequencer::Step::current() {
  return current_m.get_value();
}

