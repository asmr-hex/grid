#include <catch2/catch.hpp>

#include <opr/modules>

using namespace opr;

TEST_CASE( "opr::module::Sequence(...)" ) {
  SECTION( "(sequence_t)" ) {
    GIVEN( "a predefined sequence_t" ) {
      opr::module::Sequence<int>::sequence_t seq =
        {
         { 0, {
               {1, {1, 2, 3}},
               {2, {11, 22, 33}} } },
         { 1, {
               {1, {1, 2, 3}},
               {2, {11, 22, 33}} } },
        };
      WHEN( "a Sequence is constructed with the sequence_t" ) {
        opr::module::Sequence<int>* s = new opr::module::Sequence<int>(seq);
        THEN( "the internal sequence is initialized" ) {}
      }
    }
  }
}

TEST_CASE( "opr::module::Sequence.at(...)" ) {
  opr::module::Sequence<int>::sequence_t seq =
    {
     { 0, {
           {1, {1, 2, 3} },
           {2, {11, 22, 33} } } },
     { 2, {
           {3, {44} },
           {9, {99, 77, 88} },
           {4, {55, 66} } } },
    };
  SECTION( "(step_idx_t)") {
    GIVEN( "an unpopulated Sequence" ) {
      opr::module::Sequence<int>* s = new opr::module::Sequence<int>();
      WHEN( "any step index is accessed" ) {
        std::vector<int> steps = s->at(3);
        THEN( "an empty vector is returned" ) {
          REQUIRE( steps.size() == 0 );
        }
      }
    }
    GIVEN( "a populated Sequence ") {
      opr::module::Sequence<int>* s = new opr::module::Sequence<int>(seq);
      WHEN( "a valid step index is accessed" ) {
        std::vector<int> data = s->at(2);
        THEN( "a vector of all data at that step is returned" ) {
          REQUIRE( data.size() == 6 );
          REQUIRE( data[0] == 44 );
          REQUIRE( data[1] == 55 );
          REQUIRE( data[2] == 66 );
          REQUIRE( data[3] == 99 );
          REQUIRE( data[4] == 77 );
          REQUIRE( data[5] == 88 );
        }
      }
    }
  }
  SECTION( "(step_idx_t, step_data_id_t)") {
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int>* s = new opr::module::Sequence<int>(seq);
      WHEN( "a valid step index is accessed with an existing data id" ) {
        std::vector<int> data = s->at(2, 4);
        THEN( "the data corresponding to that data_id and step is returned") {
          REQUIRE( data.size() == 2 );
          REQUIRE( data[0] == 55 );
          REQUIRE( data[1] == 66 );
        }
      }
      WHEN( "a valid step index is accessed with a non-existing data id" ) {
        std::vector<int> data = s->at(2, 5);
        THEN( "and empty vector is returned") {
          REQUIRE( data.size() == 0 );
        }
      }
    }
  }
  SECTION( "(step_idx_t, const std::vector<step_event_id_t>&)" ) {
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int>* s = new opr::module::Sequence<int>(seq);
      WHEN( "a valid step index is accessed with an array of valid data ids" ) {
        std::vector<int> data = s->at(2, {9, 3});
        THEN( "the data corresponding to the provided data_id's and step is returned" ) {
          REQUIRE( data.size() == 4 );
        }
      }
    }
  }
}

TEST_CASE( "opr::module::Sequence.[...] (read)" ) {
  opr::module::Sequence<int>::sequence_t seq =
    {
     { 0, {
           {1, {1, 2, 3} },
           {2, {11, 22, 33} } } },
     { 2, {
           {3, {44} },
           {9, {99, 77, 88} },
           {4, {55, 66} } } },
    };
  SECTION( "[step_idx_t]" ) {
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int> s(seq);
      WHEN( "an invalid step index is accessed" ) {
        opr::module::Sequence<int>::step_t step = s[3];
        THEN( "an empty step_t map is returned" ) {
          REQUIRE( step.size() == 0 );
        }
      }
      WHEN( "a valid step index is accessed" ) {
        opr::module::Sequence<int>::step_t step = s[2];
        THEN( "the step_t map is returned" ) {
          REQUIRE( step.size() == 3 );
          REQUIRE( step[3].size() == 1 );
          REQUIRE( step[3][0] == 44 );
          REQUIRE( step[9].size() == 3 );
          REQUIRE( step[9][0] == 99 );
          REQUIRE( step[9][1] == 77 );
          REQUIRE( step[9][2] == 88 );
          REQUIRE( step[4][0] == 55 );
          REQUIRE( step[4][1] == 66 );
        }
      }
    }
    SECTION( "[step_idx_t][data_id]" ) {
      GIVEN( "a populated Sequence" ) {
        opr::module::Sequence<int> s(seq);
        WHEN( "a valid step index is accessed in two dimensions" ) {
          opr::module::Sequence<int>::data_t data = s[2][4];
          THEN( "the data map is returned" ) {
            REQUIRE( data.size() == 2 );
            REQUIRE( data[0] == 55 );
            REQUIRE( data[1] == 66 );          
          }
        }
        WHEN( "a valid step index is accessed with an invalid data id" ) {
          opr::module::Sequence<int>::data_t step = s[2][0];
          THEN( "an empty data_t map is returned" ) {
            REQUIRE( step.size() == 0 );
          }
        }
      }
    }
  }
}

TEST_CASE( "opr::module::Sequence.insert(...)" ) {
  SECTION( "(step_idx_t)" ) {
    GIVEN( "an empty Sequence" ) {
      opr::module::Sequence<int> s;
      WHEN( "a step_t is written" ) {
        s.insert(2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} });
        THEN( "the step is overwritten" ) {
          REQUIRE( s.size() == 1 );
          REQUIRE( s[2].size() == 2 );
          REQUIRE( s[2][5].size() == 3 );
          REQUIRE( s[2][5][0] == 1 );
          REQUIRE( s[2][5][1] == 2 );
          REQUIRE( s[2][5][2] == 3 );
        }
      }
    }
  }
}

TEST_CASE( "opr::module::merge(...)" ) {
  SECTION( "(step_idx_t)" ) {
    GIVEN( "an empty Sequence" ) {
      opr::module::Sequence<int> s;
      WHEN( "a step_t is written" ) {
        s.merge(2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} });
        THEN( "the step is merged" ) {
          REQUIRE( s.size() == 1 );
          REQUIRE( s[2].size() == 2 );
          REQUIRE( s[2][5].size() == 3 );
          REQUIRE( s[2][5][0] == 1 );
          REQUIRE( s[2][5][1] == 2 );
          REQUIRE( s[2][5][2] == 3 );
          REQUIRE( s[2][6].size() == 3 );
          REQUIRE( s[2][6][0] == 1 );
          REQUIRE( s[2][6][1] == 2 );
          REQUIRE( s[2][6][2] == 3 );
        }
      }
    }
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int> s({ {2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} }} });
      WHEN( "a step_t is written" ) {
        s.merge(2, { {666, {0}}, {6, {7, 9}} });
        THEN( "the step is merged" ) {
          REQUIRE( s.size() == 1 );
          REQUIRE( s[2].size() == 3 );
          REQUIRE( s[2][5].size() == 3 );
          REQUIRE( s[2][5][0] == 1 );
          REQUIRE( s[2][5][1] == 2 );
          REQUIRE( s[2][5][2] == 3 );
          REQUIRE( s[2][6].size() == 2 );
          REQUIRE( s[2][6][0] == 7 );
          REQUIRE( s[2][6][1] == 9 );
          REQUIRE( s[2][666].size() == 1 );
          REQUIRE( s[2][666][0] == 0 );
        }
      }
    }
  }
}

TEST_CASE( "opr::module::clear(...)" ) {
  SECTION( "()" ) {
    GIVEN( "an empty Sequence" ) {
      opr::module::Sequence<int> s;
      WHEN( "the sequence is cleared" ) {
        s.clear();
        THEN( "there are no more steps" ) {
          REQUIRE( s.size() == 0 );
        }
      }
    }
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int> s({ {2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} }} });
      WHEN( "the sequence is cleared" ) {
        REQUIRE( s.size() == 1 );
        s.clear();
        THEN( "there are no more steps" ) {
          REQUIRE( s.size() == 0 );
        }
      }
    }
  }
  SECTION( "(step_idx_t)" ) {
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int> s({ {2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} }}, {3, { {3, {1, 2, 3} }}} });
      WHEN( "a valid step index of the sequence is cleared" ) {
        REQUIRE( s.size() == 2 );
        s.clear(2);
        THEN( "the step provided is erased" ) {
          REQUIRE( s.size() == 1 );
          REQUIRE( s[2].size() == 0 );
          REQUIRE( s[3].size() == 1 );
        }
      }
    }
  }
  SECTION( "(step_idx_t, step_data_id_t)" ) {
    GIVEN( "a populated Sequence" ) {
      opr::module::Sequence<int> s({ {2, { {5, {1, 2, 3}}, {6, {1, 2, 3}} }} });
      WHEN( "a valid step index of the sequence is cleared" ) {
        REQUIRE( s.size() == 1 );
        REQUIRE( s[2].size() == 2 );
        s.clear(2, 6);
        THEN( "the step provided is erased" ) {
          REQUIRE( s.size() == 1 );
          REQUIRE( s[2].size() == 1 );
          REQUIRE( s[2][5].size() == 3 );
        }
      }
    }
  }
}
