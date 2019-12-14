#include <memory>

#include <catch.hpp>

#include "anemone/io/observable.hpp"
#include "anemone/io/observer.hpp"


class TestObserver : public Observer<std::string> {
public:
  virtual void handle(const std::string& event) override {
    events.push_back(event);
  }

  std::vector<std::string> events;
};

SCENARIO( "io Observers can subscribe to io Observables" ) {

  GIVEN( "an io Observable and an io Observer" ) {
    auto observer = std::make_shared<TestObserver>();
    Observable<std::string> observable;

    REQUIRE( observer->events.size() == 0 );

    WHEN( "an io Observer subscribes to the io Observable" ) {
      observer->subscribe(observable);
      
      THEN( "the io Observer is registered with the io Observable" ) {
        REQUIRE( observer.use_count() == 2 );
      }
    }
  }

  GIVEN( "an io Observer subscribed to an io Observable" ) {
    auto observer = std::make_shared<TestObserver>();
    Observable<std::string> observable;
    observer->subscribe(observable);

    REQUIRE( observer->events.size() == 0 );

    WHEN( "the io Observable broadcasts an event" ) {
      std::string msg = "buffalo buffalo buffalo buffalo buffalo";
      observable.broadcast(msg);

      THEN( "the io Observer is notified" ) {
        REQUIRE( observer->events.size() == 1 );
        REQUIRE( observer->events[0] == msg );
      }
    }

    WHEN( "the io Observable broadcasts multiple events" ) {
      std::string msg1 = "buffalo buffalo buffalo buffalo buffalo";
      std::string msg2 = "real eyes realize real lies";
      observable.broadcast(msg1);
      observable.broadcast(msg2);

      THEN( "the io Observer receives all events" ) {
        REQUIRE( observer->events.size() == 2 );
        REQUIRE( observer->events[0] == msg1 );
        REQUIRE( observer->events[1] == msg2 );
      }
    }
  }

  GIVEN( "multiple io Observers subscribed to an io Observable" ) {
    auto observer1 = std::make_shared<TestObserver>();
    auto observer2 = std::make_shared<TestObserver>();
    Observable<std::string> observable;
    
    observer1->subscribe(observable);
    observer2->subscribe(observable);

    REQUIRE( observer1.use_count() == 2 );
    REQUIRE( observer2.use_count() == 2 );

    WHEN( "the io Observable broadcasts multiple events" ) {
      std::string msg1 = "buffalo buffalo buffalo buffalo buffalo";
      std::string msg2 = "real eyes realize real lies";
      observable.broadcast(msg1);
      observable.broadcast(msg2);      

      THEN( "all io Observers receive the event" ) {
        REQUIRE( observer1->events.size() == 2 );
        REQUIRE( observer1->events[0] == msg1 );
        REQUIRE( observer1->events[1] == msg2 );

        REQUIRE( observer2->events.size() == 2 );
        REQUIRE( observer2->events[0] == msg1 );
        REQUIRE( observer2->events[1] == msg2 );
      }
    }
  }
}


SCENARIO( "io Observers can unsubscribe to io Observables" ) {

  GIVEN( "an io Observable and an io Observer" ) {
    auto observer = std::make_shared<TestObserver>();
    Observable<std::string> observable;

    REQUIRE( observer->events.size() == 0 );

    WHEN( "an io Observer unsubscribes to the io Observable" ) {
      observer->unsubscribe(observable);
      
      THEN( "nothing happens" ) {
        REQUIRE( observer.use_count() == 1 );
      }
    }
  }

  GIVEN( "an io Observer subscribed to an io Observable" ) {
    auto observer = std::make_shared<TestObserver>();
    Observable<std::string> observable;
    observer->subscribe(observable);

    REQUIRE( observer->events.size() == 0 );

    WHEN( "the io Observer unsubscribes to the io Observable" ) {
      observer->unsubscribe(observable);
      
      THEN( "the io Observer isno longer subscribed" ) {
        REQUIRE( observer.use_count() == 1 );
      }
    }

    WHEN( "the io Observer unsubscribes to the io Observable and the io Observable broadcasts" ) {
      std::string msg = "buffalo buffalo buffalo buffalo buffalo";
      observer->unsubscribe(observable);
      observable.broadcast(msg);

      THEN( "the io Observer receives no events" ) {
        REQUIRE( observer->events.size() == 0 );
      }
    }
  }

  GIVEN( "multiple io Observers subscribed to an io Observable" ) {
    auto observer1 = std::make_shared<TestObserver>();
    auto observer2 = std::make_shared<TestObserver>();
    Observable<std::string> observable;
    
    observer1->subscribe(observable);
    observer2->subscribe(observable);

    REQUIRE( observer1.use_count() == 2 );
    REQUIRE( observer2.use_count() == 2 );

    WHEN( "the io Observers unsiubscribe and the io Observable broadcasts multiple events" ) {
      std::string msg1 = "buffalo buffalo buffalo buffalo buffalo";
      std::string msg2 = "real eyes realize real lies";
      observer1->unsubscribe(observable);
      observer2->unsubscribe(observable);
      observable.broadcast(msg1);
      observable.broadcast(msg2);      

      THEN( "the io Observers receive no events" ) {
        REQUIRE( observer1->events.size() == 0 );

        REQUIRE( observer2->events.size() == 0 );
      }
    }
  }
}
