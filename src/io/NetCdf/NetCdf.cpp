#include "NetCdf.h"
#include <netcdf.h>
#include <iostream>
#include <list>
#include <chrono>
#include <ctime>
#include <iomanip>


void tsunami_lab::io::NetCdf::fillConstants(t_idx                   i_nx,
                                            t_idx                   i_ny,
                                            t_real                  i_dxy,
                                            t_real                  i_l_domainstart_y_Id,
                                            t_real                  i_domainstart_y,
                                            t_real                  i_stride,
                                            t_real          const * i_b,
                                            const char*           filename){

    int l_ncId,l_err;
    l_err = nc_open(filename,NC_WRITE, &l_ncId);

    t_real *l_coordinateX = new t_real[i_nx];
    t_real *l_coordinateY = new t_real[i_ny];
    t_real *l_temp_data_bathymetry = new t_real[(i_ny * i_nx)];
    
    for( t_idx l_iy = 0; l_iy < i_ny; l_iy++ )
    {
        l_coordinateY[l_iy] = ((l_iy + 0.5) * i_dxy )+ i_domainstart_y;
    }
    // put y coordinates
    l_err = nc_put_var_float(l_ncId, m_varIdY, l_coordinateY);
    checkNcErr(l_err,__FILE__, __LINE__);

    delete[] l_coordinateY;
    for(t_idx l_ix = 0; l_ix < i_nx; l_ix++) 
    {
        l_coordinateX[l_ix] = ((l_ix + 0.5) * i_dxy )+ i_l_domainstart_y_Id;
    }
    // put x coordinates
    l_err = nc_put_var_float(l_ncId, m_varIdX, l_coordinateX);
    checkNcErr(l_err,__FILE__, __LINE__);
    delete[] l_coordinateX;

    for( t_idx l_iy = 0; l_iy < i_ny; l_iy++)
    {
        for( t_idx l_ix = 0; l_ix < i_nx; l_ix++)
        {
            t_idx l_id = (l_iy+1) * i_stride + (l_ix+1);

            l_temp_data_bathymetry[(l_iy) * i_nx + (l_ix) ] = i_b[l_id];
        }
    }
    
    // put bathymetry values
    l_err = nc_put_var_float(l_ncId, m_varIdBathymetry, l_temp_data_bathymetry);
    checkNcErr(l_err,__FILE__, __LINE__);

    delete[] l_temp_data_bathymetry;

    //close file for now
    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

}

void tsunami_lab::io::NetCdf::read( const char* i_filename,
                                    const char* i_varname,
                                    t_idx & i_nx,
                                    t_idx & i_ny,
                                    t_real ** o_zdata,
                                    t_real ** o_xdata,
                                    t_real ** o_ydata){

    int l_ncId, l_err = 0;
    int l_varidz, l_varidy, l_varidx, l_dimX, l_dimY;

    l_err = nc_open(i_filename, NC_NOWRITE, &l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimid(l_ncId,"x",&l_dimX);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId, l_dimX, &i_nx);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimid(l_ncId,"y",&l_dimY);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId, l_dimY, &i_ny);
    checkNcErr(l_err,__FILE__, __LINE__);

    if(o_xdata != nullptr){
        nc_inq_varid(l_ncId, "x", &l_varidx);
        *o_xdata = new t_real[i_nx];
        l_err = nc_get_var_float(l_ncId, l_varidx , *o_xdata);
        checkNcErr(l_err,__FILE__, __LINE__);

    }

    if(o_ydata != nullptr){
        nc_inq_varid(l_ncId, "y", &l_varidy);
        *o_ydata = new t_real[i_ny];
        l_err = nc_get_var_float(l_ncId, l_varidy , *o_ydata);
        checkNcErr(l_err,__FILE__, __LINE__);
    }

    if(o_zdata != nullptr){
        nc_inq_varid(l_ncId, i_varname, &l_varidz);
        *o_zdata = new t_real[i_ny * i_nx ];
        l_err = nc_get_var_float(l_ncId, l_varidz , *o_zdata);
        checkNcErr(l_err,__FILE__, __LINE__);
    }

    if (nc_close(l_ncId) != NC_NOERR) {
        std::cerr << "Error closing NetCDF file: " << i_filename << std::endl;
    }
    

}

void tsunami_lab::io::NetCdf::updateFile(t_idx                i_nx,
                                         t_idx                i_ny,
                                         t_idx                i_stride,
                                         t_idx                i_time_step,
                                         t_real               i_time,
                                         t_real       const * i_h,
                                         t_real       const * i_hu,
                                         t_real       const * i_hv,
                                         const char*          filename){
    
    int l_ncId, l_err;  
    l_err = nc_open(filename,NC_WRITE,&l_ncId); 
    checkNcErr(l_err,__FILE__, __LINE__);

    std::vector<t_real> l_temp_data_height(i_ny * i_nx);

    std::vector<t_real> l_temp_data_momentum_x(i_ny * i_nx);
    std::vector<t_real> l_temp_data_momentum_y(i_ny * i_nx);

    //
    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
      for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ){

        t_idx l_id = l_iy * i_stride + l_ix;

        l_temp_data_height[(l_iy-1) * i_nx + (l_ix-1)] = i_h[l_id];
        l_temp_data_momentum_x[(l_iy-1) * i_nx + (l_ix-1)] = i_hu[l_id];
        l_temp_data_momentum_y[(l_iy-1) * i_nx + (l_ix-1)] = i_hv[l_id];
      }
    }
    
    std::vector<size_t> l_startp     = {i_time_step,0,0};
    std::vector<size_t> l_endp       = {1,i_ny,i_nx};
    std::vector<ptrdiff_t> l_stridep = {1,1,1}; //no elements get skipped
    
    l_err = nc_put_vars_float(l_ncId, m_varIdHeight, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_height.data());
    checkNcErr(l_err,__FILE__, __LINE__);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpulseX, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_x.data());
    checkNcErr(l_err,__FILE__, __LINE__);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpulseY, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_y.data());
    checkNcErr(l_err,__FILE__, __LINE__);
    //time step is how many timesteps there are and i_time what simtime it is
    l_err = nc_put_var1_float(l_ncId, m_varIdTime, &i_time_step, &i_time);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);
}

tsunami_lab::io::NetCdf::NetCdf(t_real l_nx,t_real l_ny,const char*  filename) {
    
    int l_ncId,l_err;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId, l_dimTimeId;
    int l_dim2Ids[2];
    int l_dim3Ids[3];

    
    l_err = nc_create(filename,NC_CLOBBER, &l_ncId);      
    checkNcErr(l_err,__FILE__, __LINE__); //NC_NETCDF4

    //dimensions
    l_err = nc_def_dim(l_ncId, "x", l_nx, &l_dimXId);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    l_err = nc_def_dim(l_ncId, "y", l_ny, &l_dimYId);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_dim(l_ncId, "time", NC_UNLIMITED, &l_dimTimeId);
    checkNcErr(l_err,__FILE__, __LINE__);

    //variable x
    l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &m_varIdX);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_x = "meter";
    nc_put_att_text(l_ncId, m_varIdX, "units", strlen(units_attribute_x), units_attribute_x);

    //variable y
    l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &m_varIdY);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_y = "meter";
    nc_put_att_text(l_ncId, m_varIdY, "units", strlen(units_attribute_y), units_attribute_y);

    //variable time
    l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &m_varIdTime);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_time = "seconds";
    nc_put_att_text(l_ncId, m_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);
    
    
    //variable bathymetry
    l_dim2Ids[0] =l_dimYId;
    l_dim2Ids[1] =l_dimXId;

    
    l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dim2Ids, &m_varIdBathymetry);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_Bathymetry = "meters";
    nc_put_att_text(l_ncId, m_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);
    
    
    
    //variable height

    l_dim3Ids[0] =l_dimTimeId;
    l_dim3Ids[1] =l_dimYId;
    l_dim3Ids[2] =l_dimXId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dim3Ids, &m_varIdHeight);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, m_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

    //variable momentum in x direction
    l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dim3Ids, &m_varIdImpulseX);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_impulseX = "meters";
    nc_put_att_text(l_ncId, m_varIdImpulseX, "units", strlen(units_attribute_impulseX), units_attribute_impulseX);
    
    //variable momentum in y direction 
    
    l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dim3Ids, &m_varIdImpulseY);
    checkNcErr(l_err,__FILE__, __LINE__);
    const char* units_attribute_impulseY = "meters";
    nc_put_att_text(l_ncId, m_varIdImpulseY, "units", strlen(units_attribute_impulseY), units_attribute_impulseY);
    
    //enddef
    l_err = nc_enddef( l_ncId ); 
    checkNcErr(l_err,__FILE__, __LINE__);
}

void tsunami_lab::io::NetCdf::createCheckPoint(std::string i_solver,
                                               t_real i_domain_start_x,
                                               t_real i_domain_start_y,
                                               t_real i_dimension_x,
                                               t_real i_dimension_y,
                                               t_real i_endtime,
                                               t_real i_simTime,
                                               t_real i_frequency,
                                               t_real i_dt,
                                               const t_real *i_b,
                                               const t_real *i_h,
                                               const t_real *i_hu,
                                               const t_real *i_hv,
                                               t_idx i_time_step_index,
                                               t_idx i_stride,
                                               t_idx i_nx,
                                               t_idx i_ny,
                                               std::string i_setup,
                                               std::string i_stations_string,
                                               std::string i_name_cp,
                                               std::string i_disfile,
                                               std::string i_batfile){
    
    std::string folder_path = "outputs/cp";
    std::string l_name_cp = "outputs/cp/"+i_name_cp;
    int l_err,l_ncId;
    l_err = nc_create(l_name_cp.data(), NC_NETCDF4,&l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_dimXId, l_dimYId;
    l_err = nc_def_dim(l_ncId, "x", i_nx, &l_dimXId);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_dim(l_ncId, "y", i_ny, &l_dimYId);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    int l_var_domain_start_x_id, l_var_domain_start_y_id , l_var_solver_id, l_var_dimension_x_id,
        l_var_dimension_y_id,l_var_endtime_id, l_var_simTime_id, l_var_frequency_id, l_var_b_id,
        l_var_h_id,l_var_hu_id,l_var_hv_id, l_var_dt_id , l_var_disfile_id,l_var_batfile_id;    
    int  l_var_time_step_index_id,l_var_stations_string,l_var_setup;
    //-----------------------------------------------------Define Variables
    l_err = nc_def_var(l_ncId,"domain_start_x",NC_FLOAT,0 , nullptr, &l_var_domain_start_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"domain_start_y",NC_FLOAT,0 , nullptr, &l_var_domain_start_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"solver",NC_INT,0 , nullptr, &l_var_solver_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"dimension_x",NC_FLOAT,0 , nullptr, &l_var_dimension_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"dimension_y",NC_FLOAT,0 , nullptr, &l_var_dimension_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"endtime",NC_FLOAT,0 , nullptr, &l_var_endtime_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"simTime",NC_FLOAT,0 , nullptr, &l_var_simTime_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"setup",NC_STRING,0 , nullptr, &l_var_setup);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"stations",NC_STRING,0 , nullptr, &l_var_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"timeStep",NC_INT,0 , nullptr, &l_var_time_step_index_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"frequency",NC_FLOAT,0 , nullptr, &l_var_frequency_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"dt",NC_FLOAT,0 , nullptr, &l_var_dt_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"disfile",NC_STRING,0 , nullptr, &l_var_disfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_def_var(l_ncId,"batfile",NC_STRING,0 , nullptr, &l_var_batfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    int l_dims[2] = {l_dimYId,l_dimXId};

    l_err = nc_def_var(l_ncId,"b",NC_FLOAT,2 , l_dims, &l_var_b_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"hu",NC_FLOAT,2 , l_dims, &l_var_hu_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"h",NC_FLOAT,2, l_dims, &l_var_h_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_def_var(l_ncId,"hv",NC_FLOAT,2 , l_dims, &l_var_hv_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_enddef(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

    //-----------------------------------------------------Write Variables
    const char* setupData = i_setup.data();
    l_err = nc_put_var_string(l_ncId, l_var_setup, &setupData);
    checkNcErr(l_err,__FILE__, __LINE__);

    const char* l_stations_string = i_stations_string.data();
    const char* l_disfile_string = i_disfile.data();
    const char* l_batfile_string = i_batfile.data();
    
    l_err = nc_put_var_string(l_ncId, l_var_disfile_id, &l_batfile_string);
    checkNcErr(l_err,__FILE__, __LINE__);
   
    l_err = nc_put_var_string(l_ncId, l_var_batfile_id, &l_disfile_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_string(l_ncId, l_var_stations_string, &l_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId,l_var_domain_start_x_id,&i_domain_start_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_domain_start_y_id, &i_domain_start_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dimension_x_id, &i_dimension_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dimension_y_id, &i_dimension_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_endtime_id, &i_endtime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_simTime_id, &i_simTime);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    l_err = nc_put_var_float(l_ncId, l_var_frequency_id, &i_frequency);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_put_var_float(l_ncId, l_var_dt_id, &i_dt);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_put_var_int(l_ncId, l_var_time_step_index_id, (const int*)&i_time_step_index);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_solver = 0;
    if(i_solver == "roe") l_solver = 1;
    
    l_err = nc_put_var_int(l_ncId, l_var_solver_id, &l_solver);
    checkNcErr(l_err,__FILE__, __LINE__);

    std::vector<size_t> l_start     = {0, 0};
    std::vector<size_t> l_end       = {1,i_nx};
    std::vector<ptrdiff_t> l_strid = {1,1};

    for(l_start[0] = 0; l_start[0] < i_ny; l_start[0]++){
        l_err = nc_put_vars_float(l_ncId, l_var_b_id, l_start.data(), l_end.data(),l_strid.data() , (i_b + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_h_id, l_start.data(), l_end.data(),l_strid.data() , (i_h + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_hu_id, l_start.data(), l_end.data(),l_strid.data() , (i_hu + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
        l_err = nc_put_vars_float(l_ncId, l_var_hv_id, l_start.data(), l_end.data(),l_strid.data() , (i_hv + (l_start[0]+1) * i_stride+1));
        checkNcErr(l_err,__FILE__, __LINE__);
    }

    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

}




void tsunami_lab::io::NetCdf::readCheckPoint(std::string i_path_cp,
                                            std::string & o_solver,
                                            t_real * o_domain_start_x,
                                            t_real * o_domain_start_y,
                                            t_real * o_dimension_x,
                                            t_real * o_dimension_y,
                                            t_real * o_endtime,
                                            t_real * o_simTime,
                                            t_real * o_frequency,
                                            t_real * o_dt,
                                            t_real ** o_b,
                                            t_real ** o_h,
                                            t_real ** o_hu,
                                            t_real ** o_hv,
                                            t_idx * o_time_step_index,
                                            t_idx * o_nx,
                                            t_idx * o_ny,
                                            std::string * o_setup,
                                            std::string * o_stations_string,
                                            std::string * o_disfile,
                                            std::string * o_batfile){
    
    int l_ncId = 0,l_err = 0;

    l_err = nc_open(i_path_cp.data(), NC_NOWRITE, &l_ncId); 
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_dimXId, l_dimYId;
    l_err = nc_inq_dimid(l_ncId, "x", &l_dimXId);
    checkNcErr(l_err,__FILE__, __LINE__);
    l_err = nc_inq_dimid(l_ncId, "y", &l_dimYId);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId,l_dimXId,o_nx);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_dimlen(l_ncId,l_dimYId,o_ny);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_var_domain_start_x_id, l_var_domain_start_y_id , l_var_solver_id, l_var_dimension_x_id,
        l_var_dimension_y_id,l_var_endtime_id, l_var_simTime_id, l_var_frequency_id, l_var_b_id,
        l_var_h_id,l_var_hu_id,l_var_hv_id, l_var_dt_id , l_var_disfile_id,l_var_batfile_id;    
    int  l_var_time_step_index_id,l_var_stations_string,l_var_setup;

    l_err = nc_inq_varid(l_ncId,"domain_start_x",&l_var_domain_start_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"domain_start_y",&l_var_domain_start_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"solver",&l_var_solver_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dimension_x",&l_var_dimension_x_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dimension_y",&l_var_dimension_y_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"endtime",&l_var_endtime_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"simTime",&l_var_simTime_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"setup",&l_var_setup);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"stations",&l_var_stations_string);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"timeStep",&l_var_time_step_index_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"frequency",&l_var_frequency_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"dt",&l_var_dt_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId,"disfile",&l_var_disfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);
  
    l_err = nc_inq_varid(l_ncId,"batfile",&l_var_batfile_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "b", &l_var_b_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "hu", &l_var_hu_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "h", &l_var_h_id);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_inq_varid(l_ncId, "hv", &l_var_hv_id);
    checkNcErr(l_err,__FILE__, __LINE__);


    // GET THE VARIABLES

    char *l_setup;
    l_err = nc_get_var_string(l_ncId,l_var_setup,&l_setup);
    *o_setup = std::string(l_setup);
    delete[] l_setup;
    checkNcErr(l_err,__FILE__, __LINE__);
    

    char *l_batfile;
    l_err = nc_get_var_string(l_ncId,l_var_batfile_id,&l_batfile);
    *o_batfile = std::string(l_batfile);
    delete[] l_batfile;
    checkNcErr(l_err,__FILE__, __LINE__);
    
    char *l_disfile;
    l_err = nc_get_var_string(l_ncId,l_var_disfile_id,&l_disfile);
    *o_disfile = std::string(l_disfile);
    delete[] l_disfile;
    checkNcErr(l_err,__FILE__, __LINE__);

    char *l_stations_string;
    l_err = nc_get_var_string(l_ncId,l_var_stations_string,&l_stations_string);
    *o_stations_string = std::string(l_stations_string);
    delete[] l_stations_string;
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_domain_start_x_id,o_domain_start_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_domain_start_y_id,o_domain_start_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_dimension_x_id,o_dimension_x);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_dimension_y_id,o_dimension_y);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_endtime_id,o_endtime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_simTime_id,o_simTime);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId,l_var_frequency_id,o_frequency);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId, l_var_dt_id, o_dt);
    checkNcErr(l_err,__FILE__, __LINE__);

    int l_time_step_index;
    l_err = nc_get_var_int(l_ncId, l_var_time_step_index_id, &l_time_step_index);
    *o_time_step_index = l_time_step_index;
    checkNcErr(l_err,__FILE__, __LINE__);
    int l_solver = 0;

    l_err = nc_get_var_int(l_ncId, l_var_solver_id, &l_solver);
    checkNcErr(l_err,__FILE__, __LINE__);
    
    if(l_solver == 1){
        o_solver = "roe";
    }else{
        o_solver = "fwave";  
    }

    *o_b = new t_real[*o_nx * *o_ny];
    *o_h = new t_real[*o_nx * *o_ny];
    *o_hu = new t_real[*o_nx * *o_ny];
    *o_hv = new t_real[*o_nx * *o_ny];

    l_err = nc_get_var_float(l_ncId, l_var_b_id, *o_b);
    checkNcErr(l_err,__FILE__, __LINE__); 

    l_err = nc_get_var_float(l_ncId, l_var_h_id, *o_h);
    checkNcErr(l_err,__FILE__, __LINE__); 

    l_err = nc_get_var_float(l_ncId, l_var_hu_id, *o_hu);
    checkNcErr(l_err,__FILE__, __LINE__);

    l_err = nc_get_var_float(l_ncId, l_var_hv_id, *o_hv);
    checkNcErr(l_err,__FILE__, __LINE__);  

    l_err = nc_close(l_ncId);
    checkNcErr(l_err,__FILE__, __LINE__);

}




void tsunami_lab::io::NetCdf::checkNcErr(int i_err,char const * i_file, int i_line) {
    if (i_err) {
        std::cerr << "Error: " << nc_strerror(i_err) << " , in " << i_file << " , line " << i_line << std::endl;
        exit(2);
    }
}