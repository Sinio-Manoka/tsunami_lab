#include "NetCdf.h"
#include <netcdf.h>

tsunami_lab::io::NetCdf::NetCdf(){


}


void tsunami_lab::io::NetCdf::fillXandY(t_idx                   i_nx,  
                                        t_idx                   i_ny,
                                        t_real                  i_dx,
                                        t_real                  i_dy,
                                        t_real                  i_domainstart_x,
                                        t_real                  i_domainstart_y
                                                                           ){

int l_ncId;
l_err = nc_open("output.nc",
                NC_WRITE,
                &l_ncId);

std::vector<tsunami_lab::t_real> l_coordinatX(i_nx);
std::vector<tsunami_lab::t_real> l_coordinatY(i_ny);

std::vector<size_t> startp = {0};
std::vector<size_t> endpX = {i_nx};
std::vector<size_t> endpY = {i_ny};
std::vector<ptrdiff_t> stridep = {1};

 for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
            l_coordinatX[l_ix-1] = ((l_ix-1 + 0.5) * i_dx )+ i_domainstart_x;
            l_coordinatY[l_iy-1] = ((l_iy-1 + 0.5) * i_dy )+ i_domainstart_y;
            std::cout << "l_cordiantX[" << l_ix << "]: " << l_coordinatX[l_ix] << "\t";
            std::cout << "l_cordiantY[" << l_iy << "]: " << l_coordinatY[l_iy] << "\t";

        }
    }

    l_err = nc_put_vars_float(l_ncId, l_varIdX, startp.data(), endpX.data(), stridep.data(), &l_coordinatX[0]);
    checkNcErr(l_err);
    l_err = nc_put_vars_float(l_ncId, l_varIdY, startp.data(), endpY.data(), stridep.data(), &l_coordinatY[0]);
    checkNcErr(l_err);

    l_err = nc_close(l_ncId);
    checkNcErr(l_err);

}




void tsunami_lab::io::NetCdf::generateFile(t_real l_nx,t_real l_ny) {
    
   int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId,l_dimYId,l_dimTimeId;


    std::cout << "generating netcdf-file habibi.nc " << std::endl;
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", l_nx, &l_dimXId);
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "y", l_ny, &l_dimYId);
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "time", NC_UNLIMITED, &l_dimTimeId);
    checkNcErr(l_err);

    
    l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &l_varIdX);
    checkNcErr(l_err);
    const char* units_attribute_x = "meters";
    nc_put_att_text(l_ncId, l_varIdX, "units", strlen(units_attribute_x), units_attribute_x);
    const char* axis_attribute_x = "X";
    nc_put_att_text(l_ncId, l_varIdX, "axis", strlen(axis_attribute_x), axis_attribute_x);


    l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &l_varIdY);
    checkNcErr(l_err);
    const char* units_attribute_y = "meters";
    nc_put_att_text(l_ncId, l_varIdY, "units", strlen(units_attribute_y), units_attribute_y);
    const char* axis_attribute_y = "Y";
    nc_put_att_text(l_ncId, l_varIdY, "axis", strlen(axis_attribute_y), axis_attribute_y);

    l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &l_varIdTime);
    checkNcErr(l_err);
    const char* units_attribute_time = "seconds";
    nc_put_att_text(l_ncId, l_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);

    l_dimIds[0] =l_dimYId;
    l_dimIds[1] =l_dimXId;

    l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dimIds, &l_varIdBathymetry);
    checkNcErr(l_err);
    const char* units_attribute_Bathymetry = "meters";
    nc_put_att_text(l_ncId, l_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);
    
    l_dimIds[0] =l_dimTimeId;
    l_dimIds[1] =l_dimYId;
    l_dimIds[2] =l_dimXId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dimIds, &l_varIdHeight);
    checkNcErr(l_err);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, l_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

    l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dimIds, &l_varIdImpolseX);
    checkNcErr(l_err);
    const char* units_attribute_impulseX = "meters";
    nc_put_att_text(l_ncId, l_varIdImpolseX, "units", strlen(units_attribute_impulseX), units_attribute_impulseX);

    l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dimIds, &l_varIdImpolseY);
    checkNcErr(l_err);
    const char* units_attribute_impulseY = "meters";
    nc_put_att_text(l_ncId, l_varIdImpolseY, "units", strlen(units_attribute_impulseY), units_attribute_impulseY);


    l_err = nc_enddef( l_ncId ); // ncid
    checkNcErr( l_err );
}
   /* // close file
    l_err = nc_close(l_ncId);
    checkNcErr(l_err);

    std::cout << "finished writing habibi.nc" << std::endl
    << "use ncdump to view its contents" << std::endl; */

void tsunami_lab::io::NetCdf::checkNcErr(int i_err) {
    if (i_err) {
        std::cerr << "Error: " << nc_strerror(i_err) << std::endl;
        exit(2);
    }
}