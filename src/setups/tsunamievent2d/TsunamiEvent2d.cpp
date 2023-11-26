#include "TsunamiEvent2d.h"
#include "../../io/NetCdf/NetCdf.h"
#include <cmath>
#include <cstddef> 


tsunami_lab::setups::TsunamiEvent2d::TsunamiEvent2d(t_real i_delta)
{

    tsunami_lab::io::NetCdf::read("data/artificialtsunami_bathymetry_1000.nc","z",m_bathymetry_values);
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_bathymetry_1000.nc","x",m_bathymetry_x_values);
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_bathymetry_1000.nc","y",m_bathymetry_y_values); 
 
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_displ_1000.nc","z",m_displacement_values);
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_displ_1000.nc","x",m_displacement_x_values);
    tsunami_lab::io::NetCdf::read("data/artificialtsunami_displ_1000.nc","y",m_displacement_y_values);

    m_delta = i_delta;
    
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetry( t_real i_x,
                                                                        t_real i_y) const {

    
    t_real l_batNetCdf = getBathymetryNetCdf(i_x, i_y);

    if(l_batNetCdf < 0 )
    {
        if(l_batNetCdf < -m_delta)
        {
            return l_batNetCdf + displacement(i_x, i_y);
        }
        else
        {
            return  displacement(i_x, i_y) - m_delta;
        }
    }
    else
    {
        if(l_batNetCdf < m_delta)
        {
            return  displacement(i_x, i_y) + m_delta;
        }
        else
        {
            return l_batNetCdf + displacement(i_x, i_y);
        }
    }

}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::displacement( t_real i_x,t_real i_y) const {

    if (i_x < m_displacement_x_values[0] || i_x > m_displacement_x_values[m_displacement_x_values.size() - 1] ||
        i_y < m_displacement_y_values[0] || i_y > m_displacement_y_values[m_displacement_y_values.size() - 1])
    {
        return 0;
    }
    //nachdem man schon eigentlich den index raushat wollen wir ihn nochmal berechnen????? 
    t_idx l_x = findClosestIndex(m_displacement_x_values, i_x);
    t_idx l_y = findClosestIndex(m_displacement_y_values, i_y);

    return m_displacement_values[l_x * m_displacement_y_values.size() + l_y];
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getHeight( t_real i_x,
                                                                    t_real i_y)const{

    t_real l_bin = getBathymetryNetCdf(i_x, i_y);

    if (l_bin < 0) {
        return (-l_bin > m_delta) ? -l_bin : m_delta;
    }
    return 0;
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetryNetCdf(t_real i_x, t_real i_y) const {
    //check whether the position is within our domain
    if (i_x < m_bathymetry_x_values[0] || i_x > m_bathymetry_x_values[m_bathymetry_x_values.size() - 1] ||
        i_y < m_bathymetry_y_values[0] || i_y > m_bathymetry_y_values[m_bathymetry_y_values.size() - 1])
    {
        return 0;
    }

    t_idx l_x = findClosestIndex(m_bathymetry_x_values, i_x);
    t_idx l_y = findClosestIndex(m_bathymetry_y_values, i_y);
    
    return m_bathymetry_values[l_y * m_bathymetry_x_values.size() + l_x];
}

tsunami_lab::t_idx tsunami_lab::setups::TsunamiEvent2d::findClosestIndex(const std::vector<t_real>& vec, t_real value) const {

    t_idx closestIndex = 0;
    for (t_idx index = 0; index < vec.size(); ++index) {
        if (vec[index] > value) {
            if (value - vec[index - 1] > vec[index] - value){
                closestIndex = index ;
            }
            else{
                closestIndex = index- 1;
            }
            break;
        }
    }

    return closestIndex;
}


tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumY(  t_real,
                                                                        t_real)const{
    return 0;                                                                                                                                                                      
}



tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumX(  t_real ,
                                                                        t_real)const{
    return 0;                                                                                                                       
}                    