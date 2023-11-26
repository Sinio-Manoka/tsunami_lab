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

    //m_width_bathymetry   = (m_bathymetry_values[m_bathymetry_values.size()-1]   - m_bathymetry_values[0]) / m_bathymetry_values.size();
    //m_width_displacement = (m_displacement_values[m_displacement_values.size()-1] - m_displacement_values[0])/ m_displacement_values.size();
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


    /* xD? 

    if(l_batNetCdf < -m_delta)
    {
        return l_batNetCdf + displacement(i_x, i_y);
    } 
    else if(l_batNetCdf < 0)
    {
        return -m_delta + displacement(i_x, i_y);
    }
    else
    {
        return std::max(l_batNetCdf, m_delta) + displacement(i_x, i_y);
    }
    */
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


            //          ?!?!??   l_x * m_displacement_y_values.size()  +l_y lol
    return m_displacement_values[l_y * m_displacement_x_values.size() + l_x];
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
    
    //                 !!!!!!!!!!!?? m_displacement_x_values.size()
    return m_bathymetry_values[l_y * m_bathymetry_x_values.size() + l_x];
}

tsunami_lab::t_idx tsunami_lab::setups::TsunamiEvent2d::findClosestIndex(const std::vector<t_real>& vec, t_real value) const {
    
    //find the iterator value of the first element that is bigger than "value"
    auto it = std::lower_bound(vec.begin(), vec.end(), value);

    //compute the index
    t_idx index = static_cast<t_idx>(std::distance(vec.begin(), it));

    // Adjust the index if necessary 
    /*
        ??? "(index == vec.size()||" ???? kann doch nie size sein out of boundary 
        und "lower_bound" gibt vec.end() aus also letzter index wenn keine wert größer sind
    */
    if (index > 0 &&  (std::abs(value - vec[index - 1]) < std::abs(value - vec[index]))){
        --index;
    }

    return index;
}


tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumY(  t_real,
                                                                        t_real)const{
    return 0;                                                                                                                                                                      
}



tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumX(  t_real ,
                                                                        t_real)const{
    return 0;                                                                                                                       
}                    