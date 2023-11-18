/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Unit tests for the one-dimensional wave propagation patch.
 **/
#include <catch2/catch.hpp>
#include "WavePropagation2d.h"
TEST_CASE( "Test the 2d wave propagation solver.", "[WaveProp2d]" ) {
  /*
   * Test case:
   *
   *   Single dam break problem between cell 49 and 50.
   *     left | right
   *       10 | 8
   *        0 | 0
   *
   *   Elsewhere steady state.
   *
   * The net-updates at the respective edge are given as
   * (see derivation in Roe solver):
   *    left          | right
   *      9.394671362 | -9.394671362
   *    -88.25985     | -88.25985
   */

  // construct solver and setup a dambreak problem



  tsunami_lab::patches::WavePropagation2d m_waveProp( 100 , true );

   std::size_t  l_ce;

  for( std::size_t l_ce = 0; l_ce < 100+1; l_ce++ ) {
    for( std::size_t l_cy = 0; l_cy < 100+1; l_cy++ ){
      
  
    m_waveProp.setHeight( l_ce,
                          l_cy,
                          5 );
    m_waveProp.setMomentumX( l_ce,
                             l_cy,
                             0 );
    m_waveProp.setMomentumY( l_ce,
                             l_cy,
                             0 );

     m_waveProp.setGhostOutflow(true);                         
  
    }
  }

  // set outflow boundary condition

  // perform a time step
 
    m_waveProp.setGhostOutflow(true);
    m_waveProp.timeStep(0.1);
  

  // steady state
   for( std::size_t l_cy = 1; l_cy < 50  ; l_cy++ ) {  
    for( std::size_t l_cx = 1; l_cx < 100 ; l_cx++ ) {
      l_ce = (l_cx+1)  + (l_cy+1 ) * (100+2); 
    REQUIRE( m_waveProp.getHeight()[l_ce]   == Approx( 5.0f) );
    REQUIRE( m_waveProp.getMomentumX()[l_ce] == Approx( 0.0 ) );
    }
  }

  // dam-break
  REQUIRE( m_waveProp.getHeight()[49]   == Approx(5.0) );
  REQUIRE( m_waveProp.getMomentumX()[49] == Approx( 0.0 ));

  REQUIRE( m_waveProp.getHeight()[50]   == Approx(5.0) );
  REQUIRE( m_waveProp.getMomentumX()[50] == Approx(0.0) );

  // steady state
  for( std::size_t l_ce = 50; l_ce < 100; l_ce++ ) {
    REQUIRE( m_waveProp.getHeight()[l_ce]   == Approx(5.0) );
    REQUIRE( m_waveProp.getMomentumX()[l_ce] == Approx(0.0) );
  }

}
