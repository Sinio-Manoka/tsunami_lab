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
#include <cstddef> 
#include <netcdf.h>
#include <string>
#include <eigen3/Eigen/Dense>

namespace tsunami_lab {
  namespace io {
    class NetCdf;
  }
}

class tsunami_lab::io::NetCdf {
  private:
    static void checkNcErr( int i_err);
  public:
    std::vector<Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic>> m_data_height;
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> m_data_x;
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> m_data_y;
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> m_data_bathymetry;
    
    NetCdf(t_idx                   i_nx,  
           t_idx                   i_ny);

    void saveData(t_idx                i_nx,
                  t_idx                i_ny,
                  t_idx                i_stride,
                  t_real       const * i_h);

    void generateFile(t_real               i_dy,
                      t_real               i_dx,
                      t_idx                i_nx,
                      t_idx                i_ny,
                      t_real               i_domainstart_x,
                      t_real               i_domainstart_y,
                      t_idx                i_stride,
                      t_real       const * i_b);

};

#endif
