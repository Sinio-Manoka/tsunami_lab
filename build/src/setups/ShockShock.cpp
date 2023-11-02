#include "ShockShock.h"


tsunami_lab::setups::ShockShock::ShockShock(t_real i_heightLeft,
                                            t_real i_heightRight,
                                            t_real i_huLeft,
                                            t_real i_huRight,
                                            t_real i_locationDam){
  m_heightLeft = i_heightLeft;
  m_heightRight = i_heightRight;
  m_huLeft = i_huLeft;
  m_huRight = i_huRight;
  m_locationDam = i_locationDam;
}

tsunami_lab::t_real tsunami_lab::setups::ShockShock::getHeight(t_real i_x,
                                                                  t_real      )const{
    if( i_x <= m_locationDam ) {
        return m_heightLeft;
    }
    else {
        return m_heightRight;
    }                                                                                                                         
                                                                  
}


tsunami_lab::t_real tsunami_lab::setups::ShockShock::getMomentumX(t_real i_x,
                                                                  t_real)const{
    if( i_x <= m_locationDam ) {
        return m_huLeft;
    }
    else {
        return m_huRight;
    }                                                                                                                         
                                                                  
}


tsunami_lab::t_real tsunami_lab::setups::ShockShock::getMomentumY(t_real,
                                                                  t_real)const{
    return 0;                                                                                                                                                                      
}
