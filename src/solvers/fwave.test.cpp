#include <catch2/catch.hpp>
#define private public
#include "fwave.h"
#undef public

TEST_CASE( "Testing The InverseMatrix ", "[InverseMatrix]" ) {

  float o_inverse[4];
  tsunami_lab::solvers::fwave::inverseMatrix( 2,3,o_inverse);
  REQUIRE( o_inverse[0] == 3 );
  REQUIRE( o_inverse[1] == -1 );
  REQUIRE( o_inverse[2] == -2 );
  REQUIRE( o_inverse[3] == 1 );
}

TEST_CASE( "Testing The Flux Function ", "[Flux]" ) {

  float o_inverse[2];
  tsunami_lab::solvers::fwave::flux( 9,7,5,4,o_inverse);
  REQUIRE( o_inverse[0] == -1 );
  REQUIRE( o_inverse[1] == Approx(-157.39846));

}

TEST_CASE( "Testing The eigencoefficient", "[AlphaVector]" ) {

  float i_inverse[4] = {1,2,3,4};
  float i_spalte[2] = {5,6};
  float o_eigencoefficients[2];
  tsunami_lab::solvers::fwave::eigencoefficientAlpha( i_inverse,i_spalte,o_eigencoefficients);
  REQUIRE( o_eigencoefficients[0] == 17 );
  REQUIRE( o_eigencoefficients[1] == 39);
  //60.129925
  
}

TEST_CASE( "Testing The Eigenvalues 1 ", "[EigenValues]" ) {

  float eigenVal1 = 0;
  float eigenVal2 = 0;
  tsunami_lab::solvers::fwave::eigenwerte( 5.3,
                                         2.6,
                                         -6.5660377358491,
                                         12.3846153846154,
                                         eigenVal1,
                                         eigenVal2 );

  REQUIRE( eigenVal1 == Approx( -4.9840244 ) );
  REQUIRE( eigenVal2 == Approx(  7.463668864 ) );

  tsunami_lab::solvers::fwave::eigenwerte( 10,
                                         9,
                                         -3,
                                         3,
                                         eigenVal1,
                                         eigenVal2 );

  REQUIRE( eigenVal1 == Approx( -9.7311093998375095 ) );
  REQUIRE( eigenVal2 == Approx(  9.5731051658991654 ) );


}


TEST_CASE( "Test the derivation of the Fwave net-updates ", "[Zp - Vectors]" ) {
  float i_alphas[] = {17,39};
  float i_eigens[] = {-9.7311093998375095,9.5731051658991654};
  float o_minus_A_deltaQ[2];
  float o_plus_A_deltaQ[2];
  tsunami_lab::solvers::fwave::decompose(i_alphas,
                                          i_eigens,
                                         o_minus_A_deltaQ,
                                         o_plus_A_deltaQ );

  REQUIRE( o_minus_A_deltaQ[0] == 17 );
  REQUIRE( o_minus_A_deltaQ[1] == Approx(  -165.428859797 ) );

  REQUIRE( o_plus_A_deltaQ[0] == 39 );
  REQUIRE( o_plus_A_deltaQ[1] ==  Approx(  373.35110147 ) );
}


TEST_CASE( "Testing The Decomposition ", "[Zp - Vectors]" ) {


  float l_netUpdatesL[2];
  float l_netUpdatesR[2];
  tsunami_lab::solvers::fwave::netUpdates( 10,
                                         10,
                                         0,
                                         0,
                                         l_netUpdatesL,
                                         l_netUpdatesR );

  REQUIRE( l_netUpdatesL[0] == Approx(0) );
  REQUIRE( l_netUpdatesL[1] == Approx(0) );

  REQUIRE( l_netUpdatesR[0] == Approx(0) );
  REQUIRE( l_netUpdatesR[1] == Approx(0) );


  tsunami_lab::solvers::fwave::netUpdates( 10,
                                         8,
                                         0,
                                         0,
                                         l_netUpdatesL,
                                         l_netUpdatesR ); 

  REQUIRE( l_netUpdatesL[0] ==  Approx(9.394671362) );
  REQUIRE( l_netUpdatesL[1] ==  -Approx(88.25985)    );

  REQUIRE( l_netUpdatesR[0] == -Approx(9.394671362) );
  REQUIRE( l_netUpdatesR[1] == -Approx(88.25985)    );


tsunami_lab::solvers::fwave::netUpdates( 10,
                                         9,
                                         -30,
                                         27,
                                         l_netUpdatesL,
                                         l_netUpdatesR ); 

  REQUIRE( l_netUpdatesL[0] == Approx( 33.5590017014261447899292 ) );
  REQUIRE( l_netUpdatesL[1] == Approx( -326.56631690591093200508 ) );

  REQUIRE( l_netUpdatesR[0] == Approx( 23.4409982985738561366777 ) );
  REQUIRE( l_netUpdatesR[1] == Approx( 224.403141905910928927533 ) );

}