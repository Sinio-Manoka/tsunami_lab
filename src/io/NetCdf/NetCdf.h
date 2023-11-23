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
    static void checkNcErr( int i_err);
  public:
    NetCdf();

    void saveData();

    void generateFile();

};

#endif
