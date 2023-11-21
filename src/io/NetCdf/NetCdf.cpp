#include "NetCdf.h"

void tsunami_lab::io::NetCdf::write(t_idx         i_nx,
                                    t_idx         i_ny,
                                    t_real const *i_h,
                                    t_real const *i_hu,
                                    t_real const *i_hv,
                                    t_real const *i_b) {

    // Specify the NetCDF file name
    std::string filename = "output.nc";

    // Create a new NetCDF file
    int ncid;
    nc_create(filename.c_str(), NC_CLOBBER, &ncid);

    // Define dimensions
    int x_dimid, y_dimid;
    nc_def_dim(ncid, "x", i_nx, &x_dimid);
    nc_def_dim(ncid, "y", i_ny, &y_dimid);

    // Define variables
    int h_varid, hu_varid, hv_varid, b_varid;
    nc_def_var(ncid, "h", NC_FLOAT, 1, &x_dimid, &h_varid);
    nc_def_var(ncid, "hu", NC_FLOAT, 1, &x_dimid, &hu_varid);
    nc_def_var(ncid, "hv", NC_FLOAT, 1, &x_dimid, &hv_varid);
    nc_def_var(ncid, "b", NC_FLOAT, 1, &x_dimid, &b_varid);

    // Write data to variables
    nc_put_var_float(ncid, h_varid, i_h);
    nc_put_var_float(ncid, hu_varid, i_hu);
    nc_put_var_float(ncid, hv_varid, i_hv);
    nc_put_var_float(ncid, b_varid, i_b);

    // Add attributes or other necessary information if needed

    // Close the NetCDF file
    nc_close(ncid);
}