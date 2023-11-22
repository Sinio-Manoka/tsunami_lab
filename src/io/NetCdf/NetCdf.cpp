#include "NetCdf.h"

tsunami_lab::io::NetCdf::NetCdf(){
}
tsunami_lab::io::NetCdf::~NetCdf(){
}

void tsunami_lab::io::NetCdf::saveData(){
 std::cout << "generating netcdf-file 04_simple.nc" << std::endl;
  // set up data
  int l_ncId, l_dimXId, l_dimYId, l_varId;
  int l_dimIds[2];
  int l_data[6][10];
  int l_err;
  for (std::size_t l_x = 0; l_x < 6; l_x++) { 
    for (std::size_t l_y = 0; l_y < 10; l_y++) {
      l_data[l_x][l_y] = l_x * 10 + l_y;
    }
  }

  // set up netCDF-file
  l_err = nc_create( "04_simple.nc", // path
                      NC_CLOBBER,    // cmode
                      &l_ncId );     // ncidp
  checkNcErr( l_err );

  // define dimensions
  l_err = nc_def_dim( l_ncId,      // ncid
                      "x",         // name
                      6,           // len
                      &l_dimXId ); // idp
  checkNcErr( l_err );

  l_err = nc_def_dim( l_ncId,      // ncid
                      "y",         // name
                      10,          // len
                      &l_dimYId ); // idp
  checkNcErr( l_err );

  l_dimIds[0] = l_dimXId;
  l_dimIds[1] = l_dimYId;

  l_err = nc_def_var( l_ncId,     // ncid
                      "data",     // name
                      NC_INT,     // xtype
                      2,          // ndims
                      l_dimIds,   // dimidsp
                      &l_varId ); // varidp
  checkNcErr( l_err );

  l_err = nc_enddef( l_ncId ); // ncid
  checkNcErr( l_err );

  // write data
  l_err = nc_put_var_int( l_ncId,          // ncid
                          l_varId,         // varid
                          &l_data[0][0] ); // op
  checkNcErr( l_err );

  // close file
  l_err = nc_close( l_ncId ); // ncid
  checkNcErr( l_err );

  std::cout << "finished writing 04_simple.nc" << std::endl
            << "use ncdump to view its contents" << std::endl;



}


void tsunami_lab::io::NetCdf::generateFile( t_idx        const  i_nx,
                                            t_idx        const  i_ny,
                                            t_idx                i_stride,
                                            t_real       const * i_h
                                            ) {


    std::cout << "generating netcdf-file habibi.nc" << std::endl;
    // Using a fixed-size 2D array
    const t_real habibi
    t_real l_data_height [][i_ny];

    for (t_idx l_x = 0; l_x < i_nx; l_x++) { 
        for (t_idx l_y = 0; l_y < i_ny; l_y++) {
            t_idx l_id = l_y * i_stride + l_x;
            l_data_height[l_x][l_y] = i_h[l_id];
        }
    }

    int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId;

    //variables  
    int l_varIdHeight;
    int l_err;
    int l_dimIds[2];

    //------------------------------------------------------------------------------
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", i_nx, &l_dimXId);
    l_err = nc_def_dim(l_ncId, "y", i_ny, &l_dimYId);

    checkNcErr(l_err);

    l_dimIds[1] =l_dimXId;
    l_dimIds[0] =l_dimYId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 2, l_dimIds, &l_varIdHeight);
    checkNcErr(l_err);

    l_err = nc_enddef( l_ncId ); // ncid
    checkNcErr( l_err );

    l_err = nc_put_var_float(l_ncId, l_varIdHeight,&l_data_height[0][0]);    
    checkNcErr(l_err);

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
