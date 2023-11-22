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


void tsunami_lab::io::NetCdf::generateFile( t_idx                i_nx,
                                            t_idx                i_ny,
                                            t_idx                i_stride,
                                            t_real       const * i_h
                                            ) {

    std::cout << "generating netcdf-file habibi.nc" << std::endl;
    
    t_real l_data_height_flat[i_nx][i_ny];

    for (std::size_t l_x = 0; l_x < i_nx; l_x++) { 
        for (std::size_t l_y = 0; l_y < i_ny; l_y++) {
            t_idx l_id = l_y * i_stride + l_x;
            l_data_height_flat[l_x][l_y] = i_h[l_id];
        }
    }

    int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId, l_dimTimeId;

    //variables  
    int l_varIdX, l_varIdY, l_varIdTime, l_varIdBathymetry, l_varIdHeight,l_varIdImpulseX, l_varIdImpulseY;
    int l_err;
    int l_dimIds[3];

    //------------------------------------------------------------------------------
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", 3, &l_dimXId); // 3 elements for "x"
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "y", 8, &l_dimYId); // 8 elements for "y"
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "time", 3 , &l_dimTimeId); // 3 elements for "time"
    checkNcErr(l_err);



    //SETUP THE X achse for:
    /*
     * float x(x);
     *  x:units = "meters";
     *  x:axis = "X";
    */
    
    l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &l_varIdX);
    checkNcErr(l_err);
    const char* units_attribute_x = "meters";
    nc_put_att_text(l_ncId, l_varIdX, "units", strlen(units_attribute_x), units_attribute_x);
    const char* axis_attribute_x = "X";
    nc_put_att_text(l_ncId, l_varIdX, "axis", strlen(axis_attribute_x), axis_attribute_x);


    //SETUP THE Y achse for:
    /*
     * float y(y);
     *  y:units = "meters";
     *  y:axis = "Y";
    */
    l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &l_varIdY);
    checkNcErr(l_err);
    const char* units_attribute_y = "meters";
    nc_put_att_text(l_ncId, l_varIdY, "units", strlen(units_attribute_y), units_attribute_y);
    const char* axis_attribute_y = "Y";
    nc_put_att_text(l_ncId, l_varIdY, "axis", strlen(axis_attribute_y), axis_attribute_y);

    
    //SETUP THE Time achse for:
    /*
     * float time(time);
     *  time:units = "seconds";
    */
    l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &l_varIdTime);
    checkNcErr(l_err);
    const char* units_attribute_time = "seconds";
    nc_put_att_text(l_ncId, l_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);

    l_dimIds[0] = l_dimYId;
    l_dimIds[1] = l_dimXId;
    
    //SETUP THE Bathymetry achse for:
    /*
     * float b(y,x);
     *  b:units = "meters";
    */
    l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dimIds, &l_varIdBathymetry);
    checkNcErr(l_err);
    const char* units_attribute_Bathymetry = "meters";
    nc_put_att_text(l_ncId, l_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);

    l_dimIds[2] = l_dimXId;
    l_dimIds[1] = l_dimYId;
    l_dimIds[0] = l_dimTimeId;

    //SETUP THE Height achse for:
    /*
     * float h(time,y,x);
     *  h:units = "meters";
    */
    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dimIds, &l_varIdHeight);
    checkNcErr(l_err);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, l_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);


    //SETUP THE l_varIdImpulseX achse for:
    /*
     * float hu(time,y,x);
     *  hu:units = "meters";
    */
    l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dimIds, &l_varIdImpulseX);
    checkNcErr(l_err);
    const char* units_attribute_varIdImpulseX = "meters";
    nc_put_att_text(l_ncId, l_varIdImpulseX, "units", strlen(units_attribute_varIdImpulseX), units_attribute_varIdImpulseX);


    //SETUP THE Height achse for:
    /*
     * float hv(time,y,x);
     *  hv:units = "meters";
    */
    l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dimIds, &l_varIdImpulseY);
    checkNcErr(l_err);
    const char* units_attribute_varIdImpulseY = "meters";
    nc_put_att_text(l_ncId, l_varIdImpulseY, "units", strlen(units_attribute_varIdImpulseY), units_attribute_varIdImpulseY);


    l_err = nc_enddef(l_ncId);
    checkNcErr(l_err);

    // write data
    /*
        l_err = nc_put_var_float(l_ncId, l_varIdX, l_data_x);
        checkNcErr(l_err);

        l_err = nc_put_var_float(l_ncId, l_varIdY, l_data_y);
        checkNcErr(l_err);

        l_err = nc_put_var_float(l_ncId, l_varIdTime, l_data_time);
        checkNcErr(l_err);
        
        l_err = nc_put_var_float(l_ncId, l_varIdBathymetry, l_data_Bathymetry_flat);
        checkNcErr(l_err);

        l_err = nc_put_var_float(l_ncId, l_varIdImpulseX, l_data_height_flat);
        checkNcErr(l_err);

        l_err = nc_put_var_float(l_ncId, l_varIdImpulseY, l_data_height_flat);
        checkNcErr(l_err);
    */



    l_err = nc_put_var_float(l_ncId, l_varIdHeight, l_data_height_flat);
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
