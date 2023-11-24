/**
 
 *
 * @section DESCRIPTION
 * The Shock Shock problem.
 **/
#include "TsunamiEvent2d.h"
#include "../../io/Csv/Csv.h"
#include <cmath>
#include <cstddef> 


tsunami_lab::setups::TsunamiEvent2d::TsunamiEvent2d(t_real i_delta){

    m_delta = i_delta;

    const std::string filename = "data/data_end.csv";
    std::size_t columnIndex = 3;
    m_bathymetry_values = tsunami_lab::io::Csv::read(filename,columnIndex); 
    
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetry( t_real i_x,
                                                                        t_real i_y) const {
    t_real l_bin = getBathymetryCsv(i_x);
    if(l_bin < 0 ){    
        if( l_bin < -m_delta){
            return l_bin    + displacement(i_x);  
        }else{
            return -m_delta + displacement(i_x);
        }
    }else{
       if( l_bin > m_delta){
            return l_bin    + displacement(i_x);
        }else{
            return m_delta + displacement(i_x);
        }
    } 
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getHeight( t_real i_x,
                                                                    t_real i_y     )const{
    t_real l_bin = getBathymetryCsv(i_x);                           
    if(l_bin < 0 ){
        if( -l_bin < m_delta){
            return m_delta;
        }else{
            return -l_bin;
        }
    }else{
        return 0;
    }                                                                                                                                                                                     
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::displacement( t_real i_x,t_real i_y) const{

    if((i_x > -500) && (i_y > -500) && (i_x < 500) && (i_y < 500) ){
        return 5 * fFunction(i_x) * gFunction(i_y);
    }else{
        return 0;
    }
    

}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getBathymetrynetCdf(t_real i_x) const{
    //i_x gets divided by 250 because every cell is in 250m steps
    std::size_t l_index = i_x /250; 
    return m_bathymetry_values[l_index];
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::fFunction(t_real i_x) const{
    return sin(((i_x/500)+1)* M_PI );
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::gFunction(t_real i_y) const{
    return -((i_y/500)*(i_y/500))+1;
}

tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumY(  t_real i_x,
                                                                        t_real i_y)const{
    return 0;                                                                                                                                                                      
}



tsunami_lab::t_real tsunami_lab::setups::TsunamiEvent2d::getMomentumX(  t_real i_x,
                                                                        t_real i_y)const{
    return 0;                                                                                                                       
}