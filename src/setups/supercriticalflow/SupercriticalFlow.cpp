/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional dam break problem.
 **/
#include "SupercriticalFlow.h"
#include <cmath>

tsunami_lab::setups::SupercriticalFlow::SupercriticalFlow(  t_real i_heightLeft,
                                                            t_real i_heightRight,
                                                            t_real i_locationDam) {
  m_heightLeft = i_heightLeft;
  m_heightRight = i_heightRight;
  m_locationDam = i_locationDam;
}

tsunami_lab::t_real tsunami_lab::setups::SupercriticalFlow::getHeight( t_real i_x,
                                                                t_real      ) const {
  return -getBathymetry(i_x,0);
}

tsunami_lab::t_real tsunami_lab::setups::SupercriticalFlow::getMomentumX( t_real,
                                                                   t_real ) const {
  return 0.18;
}

tsunami_lab::t_real tsunami_lab::setups::SupercriticalFlow::getMomentumY( t_real,
                                                                   t_real ) const {
  return 0;
}

tsunami_lab::t_real tsunami_lab::setups::SupercriticalFlow::getBathymetry( t_real i_x,
                                                                           t_real ) const {
  if(i_x > 8 && i_x < 12){
    return (-0.13-0.05*pow((i_x-10), 2));
  }else{
    return -0.33;
  }

}