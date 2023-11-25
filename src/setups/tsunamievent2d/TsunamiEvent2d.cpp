#include "TsunamiEvent2d.h"
#include "../../io/NetCdf/NetCdf.h"
#include <cmath>
#include <cstddef> 


tsunami_lab::setups::TsunamiEvent2d::TsunamiEvent2d(t_real i_delta){

    m_delta = i_delta;
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_bathymetry_1000.nc","z",m_bathymetry_values); 
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_displ_1000.nc","z",m_displacement_values);

}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetry( t_real i_x,
                                                                        t_real i_y) const {

    
    return i_x + i_y;
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::displacement( t_real i_x,t_real i_y) const {

    return i_x + i_y;

}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getHeight( t_real i_x,
                                                                    t_real i_y)const{

    return i_x + i_y;
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetryNetCdf(t_real i_x, t_real i_y) const{


    return i_x + i_y;
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumX( t_real,
                         t_real ) const{return 0;}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumY( t_real,
                         t_real ) const{ return 0;}
                         