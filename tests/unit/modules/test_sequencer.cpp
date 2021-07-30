#include <catch2/catch.hpp>

#include <opr/modules>


SCENARIO( "Sequencer driven by a single clock" ) {
  GIVEN( "a single clock and a Sequencer" ) {
    opr::module::Sequencer* s = new opr::module::Sequencer();
    rx::behavior<opr::tick_t> clk = rx::behavior<opr::tick_t>(opr::tick_t{});
    s->clock.sync("A", clk.get_observable().skip(1));
    s->play();

    WHEN( "the clock ticks" ) {
      clk.get_subscriber().on_next(opr::tick_t{});
      THEN( "the Sequencer advances" ) {
        REQUIRE( s->step.current.get_value() == 1 );
      }
    }
  }
}

SCENARIO( "List clocks Sequencer is synced to" ) {
    GIVEN( "multiple clocks and a Sequencer" ) {
        opr::module::Sequencer* s = new opr::module::Sequencer();
        rx::behavior<opr::tick_t> clkA = rx::behavior<opr::tick_t>(opr::tick_t{});
        rx::behavior<opr::tick_t> clkB = rx::behavior<opr::tick_t>(opr::tick_t{});
        rx::behavior<opr::tick_t> clkC = rx::behavior<opr::tick_t>(opr::tick_t{});
        s->clock.sync("A", clkA.get_observable().skip(1));
        s->clock.sync("B", clkB.get_observable().skip(1));
        s->clock.sync("C", clkC.get_observable().skip(1));
        s->play();

        WHEN( "clocks are listed" ) {
            THEN( "the names of all the clocks are returned" ) {
                REQUIRE( s->clock.list().size() == 3 );
                REQUIRE( s->clock.list()[0] == "A" );
                REQUIRE( s->clock.list()[1] == "B" );
                REQUIRE( s->clock.list()[2] == "C" );
            }
        }
    }
}

SCENARIO( "Sequencer driven by multiple clocks" ) {
  GIVEN( "two clocks and a Sequencer" ) {
    opr::module::Sequencer* s = new opr::module::Sequencer();
    rx::behavior<opr::tick_t> clkA = rx::behavior<opr::tick_t>(opr::tick_t{});
    rx::behavior<opr::tick_t> clkB = rx::behavior<opr::tick_t>(opr::tick_t{});
    s->clock.sync("A", clkA.get_observable().skip(1));
    s->clock.sync("B", clkB.get_observable().skip(1));
    s->play();

    WHEN( "clock only A ticks" ) {
      clkA.get_subscriber().on_next(opr::tick_t{});
      THEN( "the Sequencer advances" ) {
        REQUIRE( s->step.current.get_value() == 1 );
      }
    }

    WHEN( "clock only B ticks" ) {
      clkB.get_subscriber().on_next(opr::tick_t{});
      THEN( "the Sequencer advances" ) {
        REQUIRE( s->step.current.get_value() == 1 );
      }
    }

    WHEN( "clock B & A tick" ) {
      clkA.get_subscriber().on_next(opr::tick_t{});
      REQUIRE( s->step.current.get_value() == 1 );
      clkB.get_subscriber().on_next(opr::tick_t{});
      THEN( "the Sequencer advances" ) {
        REQUIRE( s->step.current.get_value() == 2 );
      }
    }

    WHEN( "clock A ticks, clock B ticks, clock B unsyncs, clock B ticks" ) {
      clkA.get_subscriber().on_next(opr::tick_t{});
      REQUIRE( s->step.current.get_value() == 1 );
      clkB.get_subscriber().on_next(opr::tick_t{});
      REQUIRE( s->step.current.get_value() == 2 );
      s->clock.unsync("B");
      clkB.get_subscriber().on_next(opr::tick_t{});
      REQUIRE( s->step.current.get_value() == 2 );
      clkA.get_subscriber().on_next(opr::tick_t{});
      THEN( "the Sequencer advances" ) {
        REQUIRE( s->step.current.get_value() == 3 );
      }
    }
  }
}
