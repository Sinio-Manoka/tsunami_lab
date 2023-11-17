/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Constants / typedefs used throughout the code.
 **/
#ifndef TSUNAMI_LAB_CONSTANTS_H
#define TSUNAMI_LAB_CONSTANTS_H

#include <cstddef>
#include <string>

namespace tsunami_lab {
  //! integral type for cell-ids, pointer arithmetic, etc.
  typedef std::size_t t_idx;

  //! floating point type
  typedef float t_real;

  struct Station {
    std::string i_name;
    tsunami_lab::t_real i_x,i_y;
    tsunami_lab::t_real i_water_height;
    tsunami_lab::t_real i_time;
  };
  
}



#endif