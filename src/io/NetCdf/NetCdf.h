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
  * @param i_err 
  *
  */
    static void checkNcErr( int i_err);
    t_real * pruneGhostCells(t_idx i_stride,t_idx i_ny,t_idx i_nx,t_real const *i_data);

  public:
  
    void updateFile(t_idx                i_nx,
                    t_idx                i_ny,
                    t_idx                i_stride,
                    t_real               i_time,
                    t_real       const * i_h,
                    t_real       const * i_hu,
                    t_real       const * i_hv);
                    
    NetCdf(t_real l_nx,t_real l_ny);

    void fillConstants(t_idx                  i_nx,  
                      t_idx                   i_ny,
                      t_real                  i_dx,
                      t_real                  i_dy,
                      t_real                  i_domainstart_x,
                      t_real                  i_domainstart_y,
                      t_real                  i_stride,
                      t_real          const * i_b);


    static int read( const char* filename,
                    const char* varname,
                    std::vector<t_real> &data);
                                
};

#endif