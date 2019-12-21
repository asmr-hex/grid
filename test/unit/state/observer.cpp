#include <catch.hpp>

#include <string>
#include <memory>
#include <variant>

#include "anemone/state/state.hpp"
#include "anemone/state/observer.hpp"


namespace TestState {
  struct SequencePage {
    int under_edit = 0;
    int last       = 1;
    std::string mode = "follow";

    bool operator==(const SequencePage& rhs) const {
      return
        under_edit == rhs.under_edit &&
        last       == rhs.last &&
        mode       == rhs.mode;
    };
  };

  struct test_composite {
    int number;
    SequencePage page;

    bool operator==(const test_composite& rhs) const {
      return
        number == rhs.number &&
        page   == rhs.page;
    };
  };
}

namespace TestAction {
  struct update_last_page {
    int last;
  };

  struct update_page_under_edit {
    int under_edit;
  };

  struct update_mode {
    std::string mode;
  };
}

class TestStateObserver : public State::Observer {
public:
  int number = 0;
  int page_under_edit = 0;
  int last_page = 1;
  std::string mode = "follow";
};


SCENARIO( "a state nodes is observable by a state observer" ) {
  using namespace State;
    
  using action_t = std::variant<TestAction::update_last_page,
                                TestAction::update_page_under_edit,
                                TestAction::update_mode>;

  auto s1 = State<TestState::SequencePage>::with_reducer<action_t>
    (TestState::SequencePage{},
     [] (TestState::SequencePage state, action_t action) -> TestState::SequencePage {
       return
         match(action,
               [&] (const TestAction::update_last_page& a) {
                 state.last = a.last;
                 return state;
               },
               [&] (const auto& a) {
                 return state;
               });
     });

  auto s2 = State<int>::with_reducer<action_t>
    (666,
     [] (int state, action_t action) -> int {
       return
         match(action,
               [&] (const TestAction::update_last_page& a) {
                 return a.last;
               },
               [&] (const auto& a) {
                 return state;
               });
     });

  auto s3 = State<TestState::test_composite>
    ::with_actions<action_t>
    ::compose<int, TestState::SequencePage>
    ([] (int number, TestState::SequencePage page) -> TestState::test_composite {
       return {
               .number = number,
               .page   = page,
       };
     }, s2, s1);

  GIVEN( "an observable state and a state observer" ) {

    auto o = std::make_shared<TestStateObserver>();

    o->subscribe<TestState::SequencePage>
      (s1,
       [&o] (TestState::SequencePage state) {
         o->last_page = state.last;
       });
    
    WHEN( "the state is updated" ) {
      REQUIRE( o->last_page == 1 );
      s1->reduce(TestAction::update_last_page{ .last = 6 });
      
      THEN( "the observer is notified" ) {
        REQUIRE( o->last_page == 6 );  
      }
    }
  }

  GIVEN( "two observable states and a state observer" ) {
    auto o = std::make_shared<TestStateObserver>();

    o->subscribe<TestState::SequencePage>
      (s1,
       [&o] (TestState::SequencePage state) {
         o->last_page = state.last;
       });
    o->subscribe<int>
      (s2,
       [&o] (int state) {
         o->number = state;
       });

    WHEN( "the both states are updated" ) {
      REQUIRE( o->last_page == 1 );
      REQUIRE( o->number == 0 );
      s1->reduce(TestAction::update_last_page{ .last = 6 });
      s2->reduce(TestAction::update_last_page{ .last = 9 });
      
      THEN( "the observer is notified of both state changes" ) {
        REQUIRE( o->last_page == 6 );
        REQUIRE( o->number == 9 );
      }
    }
  }

  GIVEN( "a composite state and a state observer" ) {
    auto o = std::make_shared<TestStateObserver>();

    o->subscribe<TestState::test_composite>
      (s3,
       [&o] (TestState::test_composite state) {
         o->last_page = state.page.last;
         o->number = state.number;
       });

    WHEN( "the both states are updated" ) {
      REQUIRE( o->last_page == 1 );
      REQUIRE( o->number == 0 );
      s3->reduce(TestAction::update_last_page{ .last = 8 });
      
      THEN( "the observer is notified of both state changes" ) {
        REQUIRE( o->last_page == 8 );
        REQUIRE( o->number == 8 );
      }
    }
  }
}
