/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional dam break problem.
 **/
#include "SubcriticalFlow.h"

tsunami_lab::setups::SubcriticalFlow::SubcriticalFlow(  t_real i_heightLeft,
                                                        t_real i_heightRight,
                                                        t_real i_locationDam,
                                                        t_real i_b) {
  m_heightLeft = i_heightLeft;
  m_heightRight = i_heightRight;
  m_locationDam = i_locationDam;
  m_b = i_b;
}

tsunami_lab::t_real tsunami_lab::setups::DamBreak1d::getHeight( t_real i_x,
                                                                t_real      ) const {
  if( i_x < m_locationDam ) {
    return m_heightLeft;
  }
  else {
    return m_heightRight;
  }
}

tsunami_lab::t_real tsunami_lab::setups::DamBreak1d::getMomentumX( t_real,
                                                                   t_real ) const {
  return 0;
}

tsunami_lab::t_real tsunami_lab::setups::DamBreak1d::getMomentumY( t_real,
                                                                   t_real ) const {
  return 0;
}

tsunami_lab::t_real tsunami_lab::setups::DamBreak1d::getBathymetry( t_real i_x,
                                                                    t_real ) const {
  if(i_x > 8 && i_x < 12){

  }else{
    return -2;
  }

}