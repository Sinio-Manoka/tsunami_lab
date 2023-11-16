/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Tests the dam break setup.
 **/
#include <catch2/catch.hpp>
#include "DamBreak2d.h"

TEST_CASE( "Test the tow-dimensional dam break setup.", "[DamBreak2d]" ) {
  tsunami_lab::setups::DamBreak2d l_damBreak;

  // left side
  REQUIRE( l_damBreak.getHeight( 2, 4 ) == 10.0 );

  REQUIRE( l_damBreak.getMomentumX( 2, 0 ) == 0 );

  REQUIRE( l_damBreak.getMomentumY( 2, 0 ) == 0 );

  REQUIRE( l_damBreak.getHeight( 2, 5 ) == 10.0  );

  REQUIRE( l_damBreak.getMomentumX( 2, 5 ) == 0 );

  REQUIRE( l_damBreak.getMomentumY( 2, 2 ) == 0 );

  // right side
  REQUIRE( l_damBreak.getHeight( 4, 0 ) == 10.0  );

  REQUIRE( l_damBreak.getMomentumX( 4, 0 ) == 0 );

  REQUIRE( l_damBreak.getMomentumY( 4, 0 ) == 0 );

  REQUIRE( l_damBreak.getHeight( 4, 5 ) == 10.0  );

  REQUIRE( l_damBreak.getMomentumX( 4, 5 ) == 0 );

  REQUIRE( l_damBreak.getMomentumY( 4, 2 ) == 0 );  
}