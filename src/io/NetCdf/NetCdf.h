/**
 * @author Khaled minawe
 *
 * @section DESCRIPTION
 * IO-routines for writing a Netcdf Files 
 **/
#ifndef TSUNAMI_LAB_IO_NETCDF
#define TSUNAMI_LAB_IO_NETCDF

#include "../../constants.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <netcdf.h>

namespace tsunami_lab {
  namespace io {
    class NetCdf;
  }
}

class tsunami_lab::io::NetCdf {
  private:
    int m_varIdX, m_varIdY,  m_varIdHeight, m_varIdTime, m_varIdBathymetry, m_varIdImpulseX, m_varIdImpulseY;
    t_idx m_time_step = 0;
  /**
  * @brief checks for errors
  * @param i_err 
  */
    static void checkNcErr( int i_err);


  public:
    /**
     * @brief adds the new time step to ne nc file
     * 
    
     * @param i_stride amount of cells in x direction + ghostzells (2).
     * @param i_time simulated time step from main
     * @param i_h height array.
     * @param i_hu momentum in x direction
     * @param i_hv momentum in y direction
     * @param filename string filename
     */
    void updateFile(t_idx                i_nx,
                    t_idx                i_ny,
                    t_idx                i_stride,
                    t_real               i_time,
                    t_real       const * i_h,
                    t_real       const * i_hu,
                    t_real       const * i_hv,
                    const char*          filename);
    
    /**
     * @brief Constructor for the NetCdf class defines the variables and the dimension and creates the file
     * 
     * @param i_nx amount of cells in x direction.
     * @param i_ny amount of cells in y direction.
     * @param filename string filename
     */
    NetCdf(t_real l_nx,t_real l_ny, const char* filename);

    /**
     * @brief  fills a NetCDF file with bathymetry data, generating X and Y coordinates based on input parameters 
     * 
     * @param i_nx amount of cells in x direction.
     * @param i_ny amount of cells in y direction.
     * @param i_dxy width of a cell.
     * @param i_domainstart_x offset in x direction.
     * @param i_domainstart_y offset in y direction.
     * @param i_stride amount of cells in x direction + ghostzells (2).
     * @param i_b bathymetry values with ghostcells.
     * @param filename string filename
     */
    void fillConstants(t_idx                  i_nx,  
                      t_idx                   i_ny,
                      t_real                  i_dxy,
                      t_real                  i_domainstart_x,
                      t_real                  i_domainstart_y,
                      t_real                  i_stride,
                      t_real          const * i_b,
                      const char*           filename);

    /**
     * @brief get the values written in a empty vector
     * 
     * @param filename string filename.
     * @param varname either "x","y","z".
     * @param data get the values written in a empty vector
     * @return -1 dummy to end method
     */
    static int read( const char* filename,
                    const char* varname,
                    std::vector<t_real> &data);
                                
};

#endif