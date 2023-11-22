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

namespace tsunami_lab {
  namespace io {
    class NetCdf;
  }
}

class tsunami_lab::io::NetCdf {
  private:
    static void checkNcErr( int i_err);

  public:
    NetCdf();
    ~NetCdf();
    void saveData();
    void generateFile(t_idx        const   i_nx,
                      t_idx        const   i_ny,
                      t_idx                i_stride,
                      t_real       const * i_h);

};

#endif
