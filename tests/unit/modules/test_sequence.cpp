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
