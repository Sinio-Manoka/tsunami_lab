#include <catch2/catch.hpp>
#include "ArtificialTsunami2d.h"

TEST_CASE( "Test the ArtificialTsunami2d setup.", "[ArtificialTsunami2d]" ) {
  tsunami_lab::setups::ArtificialTsunami2d l_ArtificialTsunami2d(20);

  // x = 500 ,y = 500  not within the if condition
  REQUIRE( l_ArtificialTsunami2d.getHeight( 500, 500 ) == 100.0f);
  //  (500 < 500) && (500 < 500) == false -> return 0 -> -100 + 0
  REQUIRE( l_ArtificialTsunami2d.getBathymetry( 500, 500 ) == -100.0f );
  
  REQUIRE( l_ArtificialTsunami2d.getMomentumX( 500, 500 ) == 0);

  REQUIRE( l_ArtificialTsunami2d.getMomentumY( 500, 500 ) == 0 );


  //x = 20 , y = 600 
  REQUIRE( l_ArtificialTsunami2d.getHeight( 20, 600) == 100.0f );
 //  (600 < 500) == false -> return 0 -> -100 + 0
  REQUIRE( l_ArtificialTsunami2d.getBathymetry( 20, 600 ) ==-100 );
  
  REQUIRE( l_ArtificialTsunami2d.getMomentumX( 20, 600 ) == 0);

  REQUIRE( l_ArtificialTsunami2d.getMomentumY( 20, 600 ) == 0 );


  //within the boundary [-500,500]x[-500,500]->R
  //x = 300 , y = 300

  /*(300 > -500) && (300 > -500) && (300 < 500) && (300 < 500) -> true
    =  m_Bin + displacement(i_x,i_y)
    
    = -100 + 5 * fFunction(i_x)             *         gFunction(i_y);
    
    = -100 + 5 * sin(((300/500)+1)* M_PI )   * (-((300/500)*(300/500))+1) 

    ca ~ 100 + -3.04338

    = -103.04338f
  */
  REQUIRE( l_ArtificialTsunami2d.getHeight( 300, 300 ) == 100.0f);
  
  REQUIRE( l_ArtificialTsunami2d.getBathymetry( 300, 300 ) == -103.04338f );
  
  REQUIRE( l_ArtificialTsunami2d.getMomentumX( 300, 300 ) == 0);

  REQUIRE( l_ArtificialTsunami2d.getMomentumY( 300, 300 ) == 0 );
  /*
    x = 10 , y = 100;
    -> both values in the domain ->

    /*(10 > -500) && (10 > -500) && (100 < 500) && (100 < 500) -> true
    =  m_Bin + displacement(i_x,i_y)
    
    = -100 + 5 * fFunction(i_x)             *         gFunction(i_y);
    
    = -100 + 5 * sin(((10/500)+1)* M_PI )   * (-((100/500)*(100/500))+1) 

    ca ~ -100 + -0.30139449

    = -100.30139449f
  */
  REQUIRE( l_ArtificialTsunami2d.getHeight( 10, 100 ) == 100.0f );

  REQUIRE( l_ArtificialTsunami2d.getBathymetry( 10, 100 ) == -100.30139449f );
  
  REQUIRE( l_ArtificialTsunami2d.getMomentumX( 10, 100 ) == 0);

  REQUIRE( l_ArtificialTsunami2d.getMomentumY( 10, 100 ) == 0 );  
} 