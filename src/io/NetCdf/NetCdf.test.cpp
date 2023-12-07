#include <catch2/catch.hpp>
#include "../../constants.h"
#include <sstream>
#include <vector>
#define private public
#include "NetCdf.h"
#include <filesystem>
#undef public

TEST_CASE( "Test the NetCdf-writer", "[NetCdfWrite]" ) {

    tsunami_lab::t_real l_h[10]  = { 0, 1, 2, 3, 4, 5, 6 ,7, 8 ,5};
    tsunami_lab::t_real l_hu[10] = { 6, 5, 4, 3, 2, 1, 0 ,7, 8 ,5};
    tsunami_lab::t_real l_hv[10] = { 6, 5, 4, 3, 2, 1, 0 ,7, 8 ,5};
    tsunami_lab::t_real l_b[10]  = { 0, 0, 0, 0, 0, 0, 0 ,7, 8 ,5};

    tsunami_lab::io::NetCdf* l_netCdf = new tsunami_lab::io::NetCdf(5,2,"testsFiles/test.nc");
    l_netCdf->fillConstants(5,2,1,-50,-50,0,l_b,"testsFiles/test.nc");
    l_netCdf->updateFile(5,2,1,0,1,l_h,l_hu,l_hv,"testsFiles/test.nc");
    int l_err;
    int l_ncidp;
    int  l_dimXId,l_dimYId,l_TimeId;
    size_t l_nx,l_ny , l_nt;
    int l_vHId, l_vXId,l_vHuId, l_vYId, l_vTimeId, l_vHvId ,l_vBId;

    REQUIRE(std::filesystem::exists("testsFiles/test.nc"));

    l_err = nc_open("testsFiles/test.nc", NC_NOWRITE, &l_ncidp);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimid(l_ncidp,"x",&l_dimXId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimid(l_ncidp,"y",&l_dimYId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err= nc_inq_dimid(l_ncidp, "time", &l_TimeId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncidp, l_dimXId, &l_nx);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncidp, l_dimYId, &l_ny);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncidp, l_TimeId, &l_nt);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    REQUIRE(l_nx == 5);
    REQUIRE(l_ny == 2);
    REQUIRE(l_nt == 1);


    l_err = nc_inq_varid(l_ncidp, "x", &l_vXId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "y", &l_vYId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "time", &l_vTimeId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "h", &l_vHId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "hu", &l_vHuId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "hv", &l_vHvId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_varid(l_ncidp, "b", &l_vBId);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);


    std::vector<tsunami_lab::t_real> x(10);
    std::vector<tsunami_lab::t_real> y(2);
    std::vector<tsunami_lab::t_real> time(1);
    std::vector<tsunami_lab::t_real> height(10);
    std::vector<tsunami_lab::t_real> momentum_x(10);
    std::vector<tsunami_lab::t_real> momentum_y(10);
    std::vector<tsunami_lab::t_real> bathymetry(10);
    
    
    l_err = nc_get_var_float(l_ncidp, l_vXId, &x[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vYId, &y[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vTimeId, &time[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vHId, &height[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vHuId, &momentum_x[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vHvId, &momentum_y[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_get_var_float(l_ncidp, l_vBId, &bathymetry[0]);
    l_netCdf->checkNcErr(l_err,__FILE__, __LINE__);

    REQUIRE(x[0] == Approx(-49.5));
    REQUIRE(x[1] == Approx(-48.5));
    REQUIRE(x[2] == Approx(-47.5));
    REQUIRE(x[3] == Approx(-46.5));
    REQUIRE(x[4] == Approx(-45.5));

     REQUIRE(y[0] == Approx(-49.5));
     REQUIRE(y[1] == Approx(-48.5));


    REQUIRE(time[0] == Approx(1));

    REQUIRE(momentum_x[0] == Approx(4));
    REQUIRE(momentum_x[1] == Approx(3));
    REQUIRE(momentum_x[2] == Approx(2));
    REQUIRE(momentum_x[3] == Approx(1));
    REQUIRE(momentum_x[4] == Approx(0));
    REQUIRE(momentum_x[5] == Approx(3));
    REQUIRE(momentum_x[6] == Approx(2));
    REQUIRE(momentum_x[7] == Approx(1));
    REQUIRE(momentum_x[8] == Approx(0));
    REQUIRE(momentum_x[9] == Approx(7));

    REQUIRE(momentum_y[0] == Approx(4));
    REQUIRE(momentum_y[1] == Approx(3));
    REQUIRE(momentum_y[2] == Approx(2));
    REQUIRE(momentum_y[3] == Approx(1));
    REQUIRE(momentum_y[4] == Approx(0));
    REQUIRE(momentum_y[5] == Approx(3));
    REQUIRE(momentum_y[6] == Approx(2));
    REQUIRE(momentum_y[7] == Approx(1));
    REQUIRE(momentum_y[8] == Approx(0));
    REQUIRE(momentum_y[9] == Approx(7));

   for (size_t i = 0; i < 10; i++)
    {
        REQUIRE(bathymetry[i] == Approx(0));
    }

    delete l_netCdf;
    
    
}


TEST_CASE( "Test the NetCdf-reader ", "[NetCdfreader]" ) {
    
    tsunami_lab::t_real * l_data;
    tsunami_lab::t_idx l_nx;
    tsunami_lab::t_idx l_ny;
    tsunami_lab::io::NetCdf::read("testsFiles/testFileForTheReader.nc", "data", l_nx, l_ny, &l_data, nullptr, nullptr) ;
    for (tsunami_lab::t_idx  i = 0; i < 71; i++)
    {
        REQUIRE(i == l_data[i]);
    }
    REQUIRE(6 == l_nx);
    REQUIRE(12 == l_ny);
    delete[] l_data; 
}