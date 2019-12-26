#ifndef TEST_FIXTURES_STATES_H
#define TEST_FIXTURES_STATES_H

namespace fixture {
  namespace state {

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
}

#endif
