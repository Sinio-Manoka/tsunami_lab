#include "NetCdf.h"
#include <netcdf.h>
#include <iostream>
#include <list>



void tsunami_lab::io::NetCdf::fillConstants(t_idx                   i_nx,
                                            t_idx                   i_ny,
                                            t_real                  i_dx,
                                            t_real                  i_dy,
                                            t_real                  i_domainstart_x,
                                            t_real                  i_domainstart_y,
                                            t_real                  i_stride,
                                            t_real          const * i_b){

int l_ncId,l_err;
l_err = nc_open("output.nc",
                NC_WRITE,
                &l_ncId);

std::vector<tsunami_lab::t_real> l_coordinatX(i_nx);
std::vector<tsunami_lab::t_real> l_coordinatY(i_ny);
std::vector<t_real> l_temp_data_bathymetry(i_ny *i_nx);
std::vector<t_real> l_temp_data_z(i_ny *i_nx);

std::vector<size_t> startp = {0};
std::vector<size_t> endpX = {i_nx};
std::vector<size_t> endpY = {i_ny};
std::vector<ptrdiff_t> stridep = {1};

 for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
            l_coordinatX[l_ix-1] = ((l_ix-1 + 0.5) * i_dx )+ i_domainstart_x;
            l_coordinatY[l_iy-1] = ((l_iy-1 + 0.5) * i_dy )+ i_domainstart_y;
            t_idx l_id = l_iy * i_stride + l_ix;
            l_temp_data_bathymetry[(l_iy-1) * i_ny + (l_ix-1)] = i_b[l_id];
            l_temp_data_z[(l_iy-1) * i_ny + (l_ix-1)] = i_b[l_id];
        }
    }

    l_err = nc_put_vars_float(l_ncId, m_varIdX, startp.data(), endpX.data(), stridep.data(), &l_coordinatX[0]);
    checkNcErr(l_err);
    l_err = nc_put_vars_float(l_ncId, m_varIdY, startp.data(), endpY.data(), stridep.data(), &l_coordinatY[0]);
    checkNcErr(l_err);
    std::vector<size_t> l_start_bathymetry     = {0,0};
    std::vector<size_t> l_end_bathymetry       = {i_ny,i_nx};
    std::vector<ptrdiff_t> l_stridep           = {1,1}; 
    l_err = nc_put_vars_float(l_ncId, m_varIdBathymetry, l_start_bathymetry.data(), l_end_bathymetry.data(), l_stridep.data(), l_temp_data_bathymetry.data());
    checkNcErr(l_err);
    l_err = nc_close(l_ncId);
    checkNcErr(l_err);

}

void tsunami_lab::io::NetCdf::updateFile(t_idx                i_nx,
                                         t_idx                i_ny,
                                         t_idx                i_stride,
                                         t_real               i_time,
                                         t_real       const * i_h,
                                         t_real       const * i_hu,
                                         t_real       const * i_hv){
    
    int l_ncId, l_err;    
    l_err = nc_open("output.nc",
                      NC_WRITE,    
                      &l_ncId); 

    checkNcErr(l_err);
    std::vector<t_real> l_temp_data_height(i_ny * i_nx);
    std::vector<t_real> l_temp_data_momentum_x(i_ny * i_nx);
    std::vector<t_real> l_temp_data_momentum_y(i_ny * i_nx);
    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
      for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {
        t_idx l_id = l_iy * i_stride + l_ix;
        l_temp_data_height[(l_iy-1) * i_nx + (l_ix-1)] = i_h[l_id];
        l_temp_data_momentum_x[(l_iy-1) * i_nx + (l_ix-1)] = i_hu[l_id];
        l_temp_data_momentum_y[(l_iy-1) * i_nx + (l_ix-1)] = i_hv[l_id];
      }
    }
    
    std::vector<size_t> l_startp     = {m_time_step,0,0};
    std::vector<size_t> l_endp       = {1,i_ny,i_nx};
    std::vector<ptrdiff_t> l_stridep = {1,1,1}; // Stride
    
    l_err = nc_put_vars_float(l_ncId, m_varIdHeight, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_height.data());
    checkNcErr(l_err);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpolseX, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_x.data());
    checkNcErr(l_err);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpolseY, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_y.data());
    checkNcErr(l_err);

    l_err = nc_put_var1_float(l_ncId, m_varIdTime, &m_time_step, &i_time);
    checkNcErr(l_err);

    l_err = nc_close(l_ncId);
    checkNcErr(l_err);
    m_time_step ++;
}

void tsunami_lab::io::NetCdf::generateFile(t_real l_nx,t_real l_ny) {
    
    int l_ncId,l_err;
    // Dimensions x, y, time 
    int l_dimXId,l_dimYId,l_dimTimeId;
    int l_dimIds[3];

    
    std::cout << "generating netcdf-file output.nc " << std::endl;
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

    
    l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &m_varIdX);
    checkNcErr(l_err);
    const char* units_attribute_x = "meters";
    nc_put_att_text(l_ncId, m_varIdX, "units", strlen(units_attribute_x), units_attribute_x);
    const char* axis_attribute_x = "X";
    nc_put_att_text(l_ncId, m_varIdX, "axis", strlen(axis_attribute_x), axis_attribute_x);


    l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &m_varIdY);
    checkNcErr(l_err);
    const char* units_attribute_y = "meters";
    nc_put_att_text(l_ncId, m_varIdY, "units", strlen(units_attribute_y), units_attribute_y);
    const char* axis_attribute_y = "Y";
    nc_put_att_text(l_ncId, m_varIdY, "axis", strlen(axis_attribute_y), axis_attribute_y);

    l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &m_varIdTime);
    checkNcErr(l_err);
    const char* units_attribute_time = "seconds";
    nc_put_att_text(l_ncId, m_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);

    l_dimIds[0] =l_dimYId;
    l_dimIds[1] =l_dimXId;

    l_err = nc_def_var(l_ncId, "z", NC_FLOAT, 2, l_dimIds, &m_varIdZ);
    checkNcErr(l_err);
    const char* units_attribute_Z = "meters";
    nc_put_att_text(l_ncId, m_varIdZ, "units", strlen(units_attribute_Z), units_attribute_Z);
    

    l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dimIds, &m_varIdBathymetry);
    checkNcErr(l_err);
    const char* units_attribute_Bathymetry = "meters";
    nc_put_att_text(l_ncId, m_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);
    
    l_dimIds[0] =l_dimTimeId;
    l_dimIds[1] =l_dimYId;
    l_dimIds[2] =l_dimXId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dimIds, &m_varIdHeight);
    checkNcErr(l_err);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, m_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

    l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dimIds, &m_varIdImpolseX);
    checkNcErr(l_err);
    const char* units_attribute_impulseX = "meters";
    nc_put_att_text(l_ncId, m_varIdImpolseX, "units", strlen(units_attribute_impulseX), units_attribute_impulseX);

    l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dimIds, &m_varIdImpolseY);
    checkNcErr(l_err);
    const char* units_attribute_impulseY = "meters";
    nc_put_att_text(l_ncId, m_varIdImpolseY, "units", strlen(units_attribute_impulseY), units_attribute_impulseY);

    l_err = nc_enddef( l_ncId ); // ncid
    checkNcErr( l_err );
}

size_t tsunami_lab::io::NetCdf::getsizeOfDimension(std::string filePath , std::string variableName){
    int l_ncId,l_err , l_dimId;
    size_t l_dim_length;
    const char* file_path = filePath.c_str();
    const char* variable_name = variableName.c_str();

        l_err = nc_open(file_path,
                      NC_NOWRITE,
                      &l_ncId);

        checkNcErr(l_err);
        l_err = nc_inq_dimid(l_ncId, variable_name, &l_dimId);
        checkNcErr(l_err);

        l_err = nc_inq_dimlen(l_ncId, l_dimId ,&l_dim_length);
        checkNcErr(l_err);

    return l_dim_length;
}


std::vector<tsunami_lab::t_real> tsunami_lab::io::NetCdf::readNetCdfDim( std::string filePath , std::string variableName ){

    int l_ncId,l_err, varid ;

    const char* file_path = filePath.c_str();
    const char* variable_name = variableName.c_str();

    std::vector<tsunami_lab::t_real> data(getsizeOfDimension(filePath,variableName));
    
    l_err = nc_open(file_path,
                      NC_NOWRITE,    
                      &l_ncId);

    checkNcErr(l_err);


    l_err = nc_inq_varid(l_ncId, variable_name , &varid);
    checkNcErr(l_err);

    
    l_err = nc_get_var_float(l_ncId, varid , &data[0]);
    checkNcErr(l_err);
    

       

    l_err = nc_close(l_ncId);

    for (tsunami_lab::t_real value : data) {
        std::cout << value << " ";
    }

    return data;
    

}

std::vector<tsunami_lab::t_real>  tsunami_lab::io::NetCdf::readNetCdfbathAndDis(std::string filePath){

     int l_ncId,l_err, varid ;

    const char* file_path = filePath.c_str();

    std::vector<tsunami_lab::t_real> data(getsizeOfDimension(filePath,"x") * getsizeOfDimension(filePath,"y"));
    
    l_err = nc_open(file_path,
                      NC_NOWRITE,    
                      &l_ncId);

    checkNcErr(l_err);

    l_err = nc_inq_varid(l_ncId, "z" , &varid);
    checkNcErr(l_err);

    
    l_err = nc_get_var_float(l_ncId, varid , &data[0]);
    checkNcErr(l_err);
       
    l_err = nc_close(l_ncId);

    for (tsunami_lab::t_real value : data) {
        std::cout << value << " ";
    }

    return data;
    

}


void tsunami_lab::io::NetCdf::checkNcErr(int i_err) {
    if (i_err) {
        std::cerr << "Error: " << nc_strerror(i_err) << std::endl;
        exit(2);
    }
}