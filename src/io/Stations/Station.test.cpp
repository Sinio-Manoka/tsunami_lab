#include <catch2/catch.hpp>
#include "../../constants.h"
#include <sstream>
#include <vector>
#define private public
#include "Station.h"
#undef public
TEST_CASE( "Test the Station IO", "[Station]" ) {
    
    tsunami_lab::t_idx l_x = 5;
    tsunami_lab::t_idx l_y = 0;
    tsunami_lab::t_idx l_time = 0;
    tsunami_lab::t_real l_water_height = 7.56f;
    std::string l_csv_path = "./test/test_data/test_station_io.csv";
    void tsunami_lab::io::Station::write(l_x,
                                         l_y,
                                         l_time,
                                         l_water_height,
                                         l_csv_path);
    

}