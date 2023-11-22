#include "NetCdf.h"
#include <eigen3/Eigen/Dense>

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


void tsunami_lab::io::NetCdf::generateFile( t_real                  i_dy,
                                            t_real                  i_dx,
                                            t_idx                   i_nx,  
                                            t_idx                   i_ny,
                                            t_real                  i_domainstart_x,
                                            t_real                  i_domainstart_y,
                                            t_idx                   i_stride,
                                            t_real          const * i_h
                                            ) {

    std::cout << "generating netcdf-file habibi.nc" << std::endl;

    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> l_data_height(i_nx, i_ny);
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> l_data_y(i_ny,1);
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> l_data_x(i_nx,1);

    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
            tsunami_lab::t_idx l_id = l_ix + i_stride * l_iy;
            t_real l_posY = ((l_iy-1 + 0.5) * i_dy )+ i_domainstart_y;
            t_real l_posX = ((l_ix-1 + 0.5) * i_dx )+ i_domainstart_x; 
                        
            if(!l_data_x.all()){
                l_data_x(l_ix-1) = l_posX;
            }

            if(!l_data_y.all()){
                l_data_y(l_iy-1) = l_posY;
            }

            l_data_height(l_ix-1, l_iy-1) = i_h[l_id];
        }
        
 
    }

    int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId;

    //variables  
    int l_varIdX, l_varIdY,  l_varIdHeight, l_varIdTime;
    int l_err;
    int l_dimIds[2];

    //------------------------------------------------------------------------------
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", i_nx, &l_dimXId); // 3 elements for "x"
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "y", i_ny, &l_dimYId); // 8 elements for "y"
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


    l_dimIds[0] =l_dimXId;
    l_dimIds[1] =l_dimYId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 2, l_dimIds, &l_varIdHeight);
    checkNcErr(l_err);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, l_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

    l_err = nc_enddef( l_ncId ); // ncid
    checkNcErr( l_err );


    l_err =    nc_put_var_float(l_ncId, l_varIdHeight, l_data_height.data());
    checkNcErr(l_err);

    l_err =    nc_put_var_float(l_ncId, l_varIdY, l_data_y.data());
    checkNcErr(l_err);

    l_err =    nc_put_var_float(l_ncId, l_varIdX, l_data_x.data());
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