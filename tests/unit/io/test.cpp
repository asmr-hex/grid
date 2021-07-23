#include <catch2/catch.hpp>


TEST_CASE("TESTING") {
    REQUIRE( 1 == 1);
}

TEST_CASE("TESTING ANOTHER") {
    REQUIRE( 2 == 2);
}

TEST_CASE("TESTING ANOTHER AGAIN") {
    REQUIRE( 2 == 2);
}

SCENARIO( "io Observers can subscribe to io Observables" ) {

    GIVEN( "an io Observable and an io Observer" ) {
        REQUIRE( 0 == 0 );

        WHEN( "an io Observer subscribes to the io Observable" ) {

            THEN( "the io Observer is registered with the io Observable" ) {
                REQUIRE( 2 == 2 );
            }
        }
    }
}
