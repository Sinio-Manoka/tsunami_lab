#include "NetCdf.h"
#include <netcdf.h>

tsunami_lab::io::NetCdf::NetCdf(){


}

void tsunami_lab::io::NetCdf::saveData( ){
    const std::string file_path = "output.nc";  // replace with your NetCDF file path

    // Open the NetCDF file for reading
    netcdf::NcFile ncFile(file_path, NcFile::read);
    // Get the dimensions
    size_t x_size = ncFile.getDim("x").getSize();
    size_t y_size = ncFile.getDim("y").getSize();
    size_t time_size = ncFile.getDim("time").getSize();

    // Read the x and y variables
    std::vector<float> x_values(x_size);
    std::vector<float> y_values(y_size);

    ncFile.getVar("x").getVar(&x_values[0]);
    ncFile.getVar("y").getVar(&y_values[0]);

    // Add x and y
    // Assuming here that the addition is element-wise
    // You might need to adjust this based on the actual operation you want to perform
    std::vector<float> h_values(x_size * y_size * time_size, 0.0);

    for (size_t t = 0; t < time_size; ++t) {
        for (size_t y = 0; y < y_size; ++y) {
            for (size_t x = 0; x < x_size; ++x) {
                size_t index = t * (x_size * y_size) + y * x_size + x;
                h_values[index] = x_values[x] + y_values[y];
            }
        }
    }

    // Access the h variable and print some values
    netCDF::NcVar h_var = ncFile.getVar("h");
    std::vector<float> h_data(x_size * y_size);
    h_var.getVar(h_data.data());

    // Print the first few values of h
    for (size_t i = 0; i < 10; ++i) {
        std::cout << "h[" << i << "] = " << h_data[i] << std::endl;
    }

}


void tsunami_lab::io::NetCdf::generateFile() {
    
    int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId,l_dimTimeId ;
    //variables  
    int l_varIdX, l_varIdY,  l_varIdHeight, l_varIdTime, l_varIdBathymetry, l_varIdImpolseX, l_varIdImpolseY;
    int l_err;
    int l_dimIds[3];


    std::cout << "generating netcdf-file habibi.nc " << std::endl;
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", 10, &l_dimXId);
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "y", 10, &l_dimYId); 
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "time", 5, &l_dimTimeId); 
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

    // close file
    l_err = nc_close(l_ncId);
    checkNcErr(l_err);

    std::cout << "finished writing habibi.nc" << std::endl
              << "use ncdump to view its contents" << std::endl;
}

void tsunami_lab::io::NetCdf::checkNcErr(int i_err) {
    if (i_err) {
        std::cerr << "Error: " << nc_strerror(i_err) << std::endl;
        exit(2);
    }
}