#ifndef TSUNAMI_LAB_IO_STATIONS
#define TSUNAMI_LAB_IO_STATIONS

#include "../../constants.h"
#include "../JsReader/Configuration.h"
#include "string"
#include <fstream>
#include <vector>

namespace tsunami_lab {
  namespace io {
    class Station;
  }
}

class tsunami_lab::io::Station{
    public:
        static void write(tsunami_lab::t_real             i_x,
                          tsunami_lab::t_real             i_dxy,
                          tsunami_lab::t_real             i_y,
                          tsunami_lab::t_idx              i_nx,
                          tsunami_lab::t_idx              i_stride,
                          tsunami_lab::t_idx              i_ny,
                          tsunami_lab::t_real const *     i_water_height,
                          std::string                     i_csv_path);
};

#endif