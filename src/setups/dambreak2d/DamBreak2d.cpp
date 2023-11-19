/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional dam break problem.
 **/
#include "DamBreak2d.h"
#include "cmath"
#include <iostream>

tsunami_lab::t_real tsunami_lab::setups::DamBreak2d::getHeight( t_real i_x,
                                                                t_real i_y) const {
    //std::cout << "getHeight" << i_y <<std::endl;
    if(std::sqrt((i_x*i_x)+(i_y*i_y)) < 10){
      return 10;
    } else{
      return 5;
    }

}

tsunami_lab::t_real tsunami_lab::setups::DamBreak2d::getMomentumX( t_real ,
                                                                   t_real ) const {
  return 0;
}



tsunami_lab::t_real tsunami_lab::setups::DamBreak2d::getMomentumY( t_real ,
                                                                   t_real ) const {
  return 0;
}

tsunami_lab::t_real tsunami_lab::setups::DamBreak2d::getBathymetry( t_real ,
                                                                    t_real ) const {
  return 0;
}