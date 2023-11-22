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
    Eigen::Matrix<tsunami_lab::t_real, Eigen::Dynamic, Eigen::Dynamic> l_data_height(i_nx, i_ny);
    static void checkNcErr( int i_err);
  public:
    NetCdf();
    ~NetCdf();
    void saveData();
    void generateFile(t_real               i_dy,
                      t_real               i_dx,
                      t_idx                i_nx,
                      t_idx                i_ny,
                      t_real               i_domainstart_x,
                      t_real               i_domainstart_y,
                      t_idx                i_stride,
                      t_real       const * i_h);

};

#endif
