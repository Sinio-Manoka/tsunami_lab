#include "NetCdf.h"


tsunami_lab::io::NetCdf::NetCdf(t_idx                   i_nx,  
                                t_idx                   i_ny){

    m_data_bathymetry.resize(i_nx, i_ny);
    m_data_x.resize(i_nx,1);
    m_data_y.resize(i_ny,1);
}

void tsunami_lab::io::NetCdf::saveData( t_idx                i_nx,
                                        t_idx                i_ny,
                                        t_idx                i_stride,
                                        t_real       const * i_h){

    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> m_temp_data_height(i_nx,i_ny);
    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
            tsunami_lab::t_idx l_id = l_ix + i_stride * l_iy;
            m_temp_data_height(l_ix-1, l_iy-1)     = i_h[l_id];
        }
    }
    m_data_height.push_back(m_temp_data_height);
}


void tsunami_lab::io::NetCdf::generateFile( t_real                  i_dy,
                                            t_real                  i_dx,
                                            t_idx                   i_nx,  
                                            t_idx                   i_ny,
                                            t_real                  i_domainstart_x,
                                            t_real                  i_domainstart_y,
                                            t_idx                   i_stride,
                                            t_real          const * i_b
                                            ) {

    int l_ncId;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId,l_dimTimeId ;
    //variables  
    int l_varIdX, l_varIdY,  l_varIdHeight, l_varIdTime,l_varIdBathymetry;
    int l_err;
    int l_dimIds[3];


    std::cout << "generating netcdf-file habibi.nc " << std::endl;
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> l_data_time(m_data_height.size(),1);
    for( t_idx i_i = 0;i_i < m_data_height.size(); i_i++ ){
        l_data_time(i_i) = i_i+1;
    }

    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
            tsunami_lab::t_idx l_id = l_ix + i_stride * l_iy;
            t_real l_posY = ((l_iy-1 + 0.5) * i_dy )+ i_domainstart_y;
            t_real l_posX = ((l_ix-1 + 0.5) * i_dx )+ i_domainstart_x; 
            if(!m_data_x.all()){
                m_data_x(l_ix-1) = l_posX;
            }

            if(!m_data_y.all()){
                m_data_y(l_iy-1) = l_posY;
            }
            m_data_bathymetry(l_ix-1, l_iy-1) = i_b[l_id];
        }
    }





    //------------------------------------------------------------------------------
    l_err = nc_create("output.nc",
                      NC_CLOBBER,    
                      &l_ncId);      
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "x", i_nx, &l_dimXId);
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "y", i_ny, &l_dimYId); 
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "time", l_data_time.size(), &l_dimTimeId); 
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

    l_err = nc_enddef( l_ncId ); // ncid
    checkNcErr( l_err );

    l_err =    nc_put_var_float(l_ncId, l_varIdY, m_data_y.data());
    checkNcErr(l_err);

    l_err =    nc_put_var_float(l_ncId, l_varIdX, m_data_x.data());
    checkNcErr(l_err);

    l_err =    nc_put_var_float(l_ncId, l_varIdTime, l_data_time.data());
    checkNcErr(l_err);

    l_err =    nc_put_var_float(l_ncId, l_varIdBathymetry, m_data_bathymetry.data());
    checkNcErr(l_err);


    l_err =    nc_put_var_float(l_ncId, l_varIdHeight, m_data_height[0].data());
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