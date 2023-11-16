/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Unit tests for the CSV-interface.
 **/
#include <catch2/catch.hpp>
#include "../../constants.h"
#include <sstream>
#include <vector>
#define private public
#include "Csv.h"
#undef public

TEST_CASE( "Test the CSV-writer for 1D settings.", "[CsvWrite1d]" ) {
  // define a simple example
  tsunami_lab::t_real l_h[7]  = { 0, 1, 2, 3, 4, 5, 6 };
  tsunami_lab::t_real l_hu[7] = { 6, 5, 4, 3, 2, 1, 0 };

  std::stringstream l_stream0;
  tsunami_lab::io::Csv::write( 0.5,
                               5,
                               1,
                               7,
                               l_h+1,
                               l_hu+1,
                               0,
                               nullptr,
                               l_stream0 );

  std::string l_ref0 = R"V0G0N(x,y,height,momentum_x
-49.75,-49.75,5,0
-49.25,-49.75,4,0
-48.75,-49.75,3,0
-48.25,-49.75,2,0
-47.75,-49.75,1,0
)V0G0N";

  REQUIRE( l_stream0.str().size() == l_ref0.size() );
  REQUIRE( l_stream0.str() == l_ref0 );
}

TEST_CASE( "Test the CSV-writer for 2D settings.", "[CsvWrite2d]" ) {
  // define a simple example
  tsunami_lab::t_real l_h[16]  = {  0,  1,  2,  3,
                                    4,  5,  6,  7,
                                    8,  9, 10, 11,
                                   12, 13, 14, 15 };
  tsunami_lab::t_real l_hu[16] = { 15, 14, 13, 12,
                                   11, 10,  9,  8,
                                    7,  6,  5,  4,
                                    3,  2,  1,  0 };
  tsunami_lab::t_real l_hv[16] = {  0,  4,  8, 12,
                                    1,  5,  9, 13,
                                    2,  6, 10, 14,
                                    3,  7, 11, 15 };

  std::stringstream l_stream1;
  tsunami_lab::io::Csv::write( 10,
                               2,
                               2,
                               4,
                               l_h+4+1,
                               l_hu+4+1,
                               l_hv+4+1,
                               0,
                               l_stream1 );

std::string l_ref1 = R"V0G0N(x,y,height,momentum_x,momentum_y
-45,-45,10,5,10
-35,-45,11,4,14
-45,-35,14,1,11
-35,-35,15,0,15
)V0G0N";

  REQUIRE( l_stream1.str().size() == l_ref1.size() );
  REQUIRE( l_stream1.str() == l_ref1 );
}

TEST_CASE( "Test the CSV-reader for 1D settings.", "[CsvRead1d]" ) {

  const std::string filename = "data/data_end.csv";
  std::size_t columnIndex = 3;
  std::vector<tsunami_lab::t_real> selectedColumn = tsunami_lab::io::Csv::read(filename,columnIndex);

  REQUIRE(selectedColumn[0] == Approx(-8.39972685779));
  REQUIRE(selectedColumn[1] == Approx(-5.16040036612));
}