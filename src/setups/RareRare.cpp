#include "RareRare.h"


tsunami_lab::setups::RareRare::RareRare(t_real i_heightLeft,
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

tsunami_lab::t_real tsunami_lab::setups::RareRare::getHeight(t_real i_x,
                                                                  t_real      )const{
    if( i_x <= m_locationDam ) {
        return m_heightRight;
    }
    else {
        return m_heightLeft;
    }                                                                                                                         
                                                                  
}


tsunami_lab::t_real tsunami_lab::setups::RareRare::getMomentumX(t_real i_x,
                                                                  t_real)const{
    if( i_x <= m_locationDam ) {
        return m_huRight;
    }
    else {
        return m_huLeft;
    }                                                                                                                         
                                                                  
}


tsunami_lab::t_real tsunami_lab::setups::RareRare::getMomentumY(t_real,
                                                                  t_real)const{
    return 0;                                                                                                                                                                      
}
