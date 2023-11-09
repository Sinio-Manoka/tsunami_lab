/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Tests the dam break setup.
 **/
#include <catch2/catch.hpp>
#include "SubcriticalFlow.h"

TEST_CASE( "Test the one-dimensional dam break setup.", "[SubcriticalFlow]" ) {
  tsunami_lab::setups::SubcriticalFlow l_subcriticalFlow( 25,
                                              55,
                                               3);

  // left side
  REQUIRE( l_subcriticalFlow.getHeight( 2, 0 ) == 25 );

  REQUIRE( l_subcriticalFlow.getMomentumX( 2, 0 ) == 0 );

  REQUIRE( l_subcriticalFlow.getMomentumY( 2, 0 ) == 0 );

  REQUIRE( l_subcriticalFlow.getHeight( 2, 5 ) == 25 );

  REQUIRE( l_subcriticalFlow.getMomentumX( 2, 5 ) == 0 );

  REQUIRE( l_subcriticalFlow.getMomentumY( 2, 2 ) == 0 );

  // right side
  REQUIRE( l_subcriticalFlow.getHeight( 4, 0 ) == 55 );

  REQUIRE( l_subcriticalFlow.getMomentumX( 4, 0 ) == 0 );

  REQUIRE( l_subcriticalFlow.getMomentumY( 4, 0 ) == 0 );

  REQUIRE( l_subcriticalFlow.getHeight( 4, 5 ) == 55 );

  REQUIRE( l_subcriticalFlow.getMomentumX( 4, 5 ) == 0 );

  REQUIRE( l_subcriticalFlow.getMomentumY( 4, 2 ) == 0 );  
}