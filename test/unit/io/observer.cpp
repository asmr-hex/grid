#include <memory>

#include <catch.hpp>

#include "anemone/io/observer.hpp"
#include "anemone/io/observable.hpp"


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
      
      THEN( "the io Observer is registered to the io Observable" ) {
        REQUIRE( observer.use_count() == 2 );
      }

    }
  }
  
}


