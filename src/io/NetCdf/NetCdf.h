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
        //variables
    int l_varIdX, l_varIdY,  l_varIdHeight, l_varIdTime, l_varIdBathymetry, l_varIdImpulsX, l_varIdImpulsY;
    int l_err;
    int l_dimIds[3];

  public:
    NetCdf();

    void saveData();

    void generateFile(t_real l_nx,t_real l_ny);

    void fillXandY(t_idx                   i_nx,  
                   t_idx                   i_ny,
                   t_real                  i_dx,
                  t_real                    i_dy,
                   t_real                  i_domainstart_x,
                   t_real                  i_domainstart_y);





                                

};

#endif
