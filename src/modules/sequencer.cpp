#include <opr/modules>

#include <iostream>

using namespace opr::module;

Sequencer::Sequencer() : clock(*this) {}

void Sequencer::play() {
  playback.playing.get_subscriber().on_next(true);
  playback.stopped.get_subscriber().on_next(false);
  playback.paused.get_subscriber().on_next(false);
}

void Sequencer::stop() {
  playback.playing.get_subscriber().on_next(false);
  playback.stopped.get_subscriber().on_next(true);
  playback.paused.get_subscriber().on_next(false);
}

void Sequencer::pause() {
  playback.playing.get_subscriber().on_next(false);
  playback.stopped.get_subscriber().on_next(false);
  playback.paused.get_subscriber().on_next(true);
}

Sequencer::Clock::Clock(Sequencer& s) : sequencer(s) {}

void Sequencer::Clock::sync(std::string name, rx::observable<bool> clk) {
    // add new clock to clocks mapping
    clocks[name] = clk;

    // merge clock streams
    observable = clocks.size() == 1
        ? clk
        : static_cast<rx::observable<bool>>(observable.merge(clk));

    subscribe();
}

void Sequencer::Clock::unsync(std::string name) {
    // remove clock from mapping
    clocks.erase(name);

    // unsubscribe & return early if there are no more clocks
    if (clocks.size() == 0 ) {
        subscription.unsubscribe();
        return;
    }

    // create new merged subscription
    bool init = false;
    for (auto itr : clocks) {
      if (!init) {
          observable = itr.second;
          init = true;
          continue;
      }
      observable = observable.merge(itr.second);
    }

    // re-subscribe to clocks
    subscribe();
}

void Sequencer::Clock::subscribe() {
  // always unsubscribe before subscribing (TODO: edge case when we haven't subscribed yet)
  subscription.unsubscribe();

  // subscribe to clock ticks
  subscription = observable
    .subscribe([this] (bool _) {
      if (!sequencer.playback.playing.get_value()) return;
      auto current = sequencer.step.current.get_value();
      auto stride  = sequencer.step.stride.get_value();
      sequencer.step.current
        .get_subscriber()
        .on_next(current + stride);
    });
}

std::vector<std::string> Sequencer::Clock::list() {
    std::vector<std::string> names;
    for (auto itr : clocks) {
        names.push_back(itr.first);
    }
    return names;
}
