#ifndef TEST_FIXTURES_OBSERVERS_H
#define TEST_FIXTURES_OBSERVERS_H

#include <string>
#include <vector>

#include "anemone/io/observer.hpp"
#include "anemone/io/grid/event.hpp"



class TestStringObserver : public Observer<std::string> {
public:
  virtual void handle(const std::string& event) override {
    events.push_back(event);
  }

  std::vector<std::string> events;
};


class TestGridEventObserver : public Observer<grid_event_t> {
public:
  virtual void handle(const grid_event_t& event) override {
    events.push_back(event);
  }

  std::vector<grid_event_t> events;
};


#endif
