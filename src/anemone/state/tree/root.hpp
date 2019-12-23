#ifndef STATE_TREE_ROOT_H
#define STATE_TREE_ROOT_H

#include <thread>
#include <memory>

#include "anemone/util/concurrent_queue.hpp"
#include "anemone/state/state.hpp"
#include "anemone/state/tree/node.hpp"
#include "anemone/state/tree/sequencer/state.hpp"


namespace State {
  namespace Tree {

    // define composite root type
    struct root_t {
      sequencer_t           sequencer;
      rendered_instrument_t rendered_instrument;

      bool operator==(const root_t& rhs) {
        return
          sequencer           == rhs.sequencer &&
          rendered_instrument == rhs.rendered_instrument;
      };
    };
    
    class Root : public Node<root_t> {
    public:
      std::shared_ptr<Sequencer> sequencer;
      std::shared_ptr<RenderedInstruments> rendered_instrument;

      Root(std::shared_ptr<Queue<action_t> > q) : queue(q) {
        sequencer = std::make_shared<Sequencer>();
        rendered_instruments = std::make_shared<RenderedInstruments>();

        using namespace State;
        
        // instantiate root composite state
        state = State<root_t>
          ::with_actions<action_t>
          ::compose<sequencer_t, rendered_instrument_t>
          ([] (sequencer_t s, rendered_instrument_t i) -> root_t {
             return {
                     .sequencer           = s,
                     .rendered_instrument = i,
             };
           });
      };

      void listen_for_actions() {
        std::thread t([this]{
                        while (true) {
                          action_t action = queue.pop();
                          state->reduce(action);                          
                        }
                      });

        t.detach();
      };
    private:
      std::shared_ptr<Queue<action_t> > queue;
    };
    
  }
}

#endif
