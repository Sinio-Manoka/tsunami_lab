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
    l_err = nc_open("output.nc",NC_WRITE, &l_ncId);

    t_real *l_coordinateX = new t_real[i_nx];
    t_real *l_coordinateY = new t_real[i_ny];
    t_real *l_temp_data_bathymetry = new t_real[(i_ny * i_nx)];
    
    for( t_idx l_iy = 0; l_iy < i_ny; l_iy++ )
    {
        l_coordinateY[l_iy] = ((l_iy + 0.5) * i_dy )+ i_domainstart_y;
    }
    // put y coordinates
    l_err = nc_put_var_float(l_ncId, m_varIdY, l_coordinateY);
    checkNcErr(l_err);

    delete[] l_coordinateY;
    for(t_idx l_ix = 0; l_ix < i_nx; l_ix++) 
    {
        l_coordinateX[l_ix] = ((l_ix + 0.5) * i_dx )+ i_domainstart_x;
    }
    // put x coordinates
    l_err = nc_put_var_float(l_ncId, m_varIdX, l_coordinateX);
    checkNcErr(l_err);
    delete[] l_coordinateX;

    for( t_idx l_iy = 0; l_iy < i_ny; l_iy++ )
    {
        for( t_idx l_ix = 0; l_ix < i_nx; l_ix++)
        {   
            t_idx l_id = (l_iy) * i_stride + (l_ix+1);

            l_temp_data_bathymetry[(l_iy * i_nx) + l_ix] = i_b[l_id];
        }
    }
    
    // put bathymetry values
    l_err = nc_put_var_float(l_ncId, m_varIdBathymetry, l_temp_data_bathymetry);
    checkNcErr(l_err);

    delete[] l_temp_data_bathymetry;

    //close file for now
    l_err = nc_close(l_ncId);
    checkNcErr(l_err);

}

int tsunami_lab::io::NetCdf::read(  const char* filename,
                                    const char* varname,
                                    std::vector<t_real>  & data){
    int l_ncId; 
    //if an error occurs it return -1
    if (nc_open(filename, NC_NOWRITE, &l_ncId) != NC_NOERR) {
        std::cerr << "Error opening NetCDF file: " << filename << std::endl;
        return -1;
    }

    int varid; 
    //get the variable identifier
    if (nc_inq_varid(l_ncId, varname, &varid) != NC_NOERR) {
        std::cerr << "Error getting variable ID for variable: " << varname << std::endl;
        nc_close(l_ncId);
        return -1; 
    }

    int ndims; 
    int dimids[NC_MAX_VAR_DIMS];
    // get amount of dimensions (ndims) and the identifiers of the dimensions (dimids)
    if (nc_inq_var(l_ncId, varid, nullptr, nullptr, &ndims, dimids, nullptr) != NC_NOERR) {
        std::cerr << "Error getting variable information for variable: " << varname << std::endl;
        nc_close(l_ncId);
        return -1;
    }

    //get the length of the dimension
    t_idx dataSize = 1;
    for (int i = 0; i < ndims; ++i) {
        char dimname[NC_MAX_NAME + 1];
        size_t dimlen;
        if (nc_inq_dim(l_ncId, dimids[i], dimname, &dimlen) != NC_NOERR) {
            std::cerr << "Error getting dimension information." << std::endl;
            nc_close(l_ncId); 
        }
        dataSize *= dimlen;
    }
    //write the data in the vector
    data.resize(dataSize);
    int l_err = nc_get_var_float(l_ncId, varid , &data[0]);
    checkNcErr(l_err);

    if (nc_close(l_ncId) != NC_NOERR) {
        std::cerr << "Error closing NetCDF file: " << filename << std::endl;
    }
    return -1;
}

void tsunami_lab::io::NetCdf::updateFile(t_idx                i_nx,
                                         t_idx                i_ny,
                                         t_idx                i_stride,
                                         t_real               i_time,
                                         t_real       const * i_h,
                                         t_real       const * i_hu,
                                         t_real       const * i_hv){
    
    int l_ncId, l_err;  
    //open file
    l_err = nc_open("output.nc",NC_WRITE,&l_ncId); 
    checkNcErr(l_err);

    //create memory
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
    
    std::vector<size_t> l_startp     = {m_time_step,0,0};
    std::vector<size_t> l_endp       = {1,i_ny,i_nx};
    std::vector<ptrdiff_t> l_stridep = {1,1,1}; //no elements get skipped
    
    l_err = nc_put_vars_float(l_ncId, m_varIdHeight, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_height.data());
    checkNcErr(l_err);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpulseX, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_x.data());
    checkNcErr(l_err);
        
    l_err = nc_put_vars_float(l_ncId, m_varIdImpulseY, l_startp.data(), l_endp.data(), l_stridep.data(), l_temp_data_momentum_y.data());
    checkNcErr(l_err);
    //time step is how many timesteps there are and i_time what simtime it is
    l_err = nc_put_var1_float(l_ncId, m_varIdTime, &m_time_step, &i_time);
    checkNcErr(l_err);

    l_err = nc_close(l_ncId);
    checkNcErr(l_err);
    m_time_step ++;
}

tsunami_lab::io::NetCdf::NetCdf(t_real l_nx,t_real l_ny) {
    
    int l_ncId,l_err;
    // Dimensions x, y, time 
    int l_dimXId, l_dimYId, l_dimTimeId;
    int l_dim2Ids[2];
    int l_dim3Ids[3];

    

    std::cout << "generating netcdf-file output.nc " << std::endl;

    l_err = nc_create("output.nc",NC_CLOBBER, &l_ncId);      
    checkNcErr(l_err);

    //dimensions
    l_err = nc_def_dim(l_ncId, "x", l_nx, &l_dimXId);
    checkNcErr(l_err);
    
    l_err = nc_def_dim(l_ncId, "y", l_ny, &l_dimYId);
    checkNcErr(l_err);

    l_err = nc_def_dim(l_ncId, "time", NC_UNLIMITED, &l_dimTimeId);
    checkNcErr(l_err);

    //variable x
    l_err = nc_def_var(l_ncId, "x", NC_FLOAT, 1, &l_dimXId, &m_varIdX);
    checkNcErr(l_err);
    const char* units_attribute_x = "meters";
    nc_put_att_text(l_ncId, m_varIdX, "units", strlen(units_attribute_x), units_attribute_x);
    const char* axis_attribute_x = "X";
    nc_put_att_text(l_ncId, m_varIdX, "axis", strlen(axis_attribute_x), axis_attribute_x);

    //variable y
    l_err = nc_def_var(l_ncId, "y", NC_FLOAT, 1, &l_dimYId, &m_varIdY);
    checkNcErr(l_err);
    const char* units_attribute_y = "meters";
    nc_put_att_text(l_ncId, m_varIdY, "units", strlen(units_attribute_y), units_attribute_y);
    const char* axis_attribute_y = "Y";
    nc_put_att_text(l_ncId, m_varIdY, "axis", strlen(axis_attribute_y), axis_attribute_y);

    //variable time
    l_err = nc_def_var(l_ncId, "time", NC_FLOAT, 1, &l_dimTimeId, &m_varIdTime);
    checkNcErr(l_err);
    const char* units_attribute_time = "seconds";
    nc_put_att_text(l_ncId, m_varIdTime, "units", strlen(units_attribute_time), units_attribute_time);
    
    
    //variable bathymetry
    l_dim2Ids[0] =l_dimYId;
    l_dim2Ids[1] =l_dimXId;

    
    l_err = nc_def_var(l_ncId, "b", NC_FLOAT, 2, l_dim2Ids, &m_varIdBathymetry);
    checkNcErr(l_err);
    const char* units_attribute_Bathymetry = "meters";
    nc_put_att_text(l_ncId, m_varIdBathymetry, "units", strlen(units_attribute_Bathymetry), units_attribute_Bathymetry);
    
    
    
    //variable height

    l_dim3Ids[0] =l_dimTimeId;
    l_dim3Ids[1] =l_dimYId;
    l_dim3Ids[2] =l_dimXId;

    l_err = nc_def_var(l_ncId, "h", NC_FLOAT, 3, l_dim3Ids, &m_varIdHeight);
    checkNcErr(l_err);
    const char* units_attribute_height = "meters";
    nc_put_att_text(l_ncId, m_varIdHeight, "units", strlen(units_attribute_height), units_attribute_height);

    //variable momentum in x direction
    l_err = nc_def_var(l_ncId, "hu", NC_FLOAT, 3, l_dim3Ids, &m_varIdImpulseX);
    checkNcErr(l_err);
    const char* units_attribute_impulseX = "meters";
    nc_put_att_text(l_ncId, m_varIdImpulseX, "units", strlen(units_attribute_impulseX), units_attribute_impulseX);
    
    //variable momentum in y direction 
    
    l_err = nc_def_var(l_ncId, "hv", NC_FLOAT, 3, l_dim3Ids, &m_varIdImpulseY);
    checkNcErr(l_err);
    const char* units_attribute_impulseY = "meters";
    nc_put_att_text(l_ncId, m_varIdImpulseY, "units", strlen(units_attribute_impulseY), units_attribute_impulseY);
    
    //enddef
    l_err = nc_enddef( l_ncId ); 
    checkNcErr( l_err );
}

void tsunami_lab::io::NetCdf::checkNcErr(int i_err) {
    if (i_err) {
        std::cerr << "Error: " << nc_strerror(i_err) << std::endl;
        exit(2);
    }
}