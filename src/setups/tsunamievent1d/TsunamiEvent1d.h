/**
 * @author Ward Tammaa 
 *
 * @section DESCRIPTION
 * supercriticalflow.
 **/
#ifndef TSUNAMI_LAB_SETUPS_TSUNAMIEVENT1D_H
#define TSUNAMI_LAB_SETUPS_TSUNAMIEVENT1D_H

#include "../Setup.h"
#include <fstream>
#include <sstream>
#include <vector>


namespace tsunami_lab {
  namespace setups {
    class TsunamiEvent1d;
  }
}

/**
 * SupercriticalFlow setup.
 **/
class tsunami_lab::setups::TsunamiEvent1d: public Setup {

  private:
    
    t_real m_delta = 0; 

    std::vector<t_real> m_raws;

    t_real displacement( t_real i_x) const;

  public:

    /**
     * Constructor.
     *
     * @param i_delta small delta!!
     **/

    TsunamiEvent1d( t_real i_delta);

    /**
     * Gets the water height at a given point.
     *
     * @param i_x x-coordinate of the queried point.
     * @return height at the given point.
     **/
    t_real getHeight( t_real i_x,
                      t_real      ) const;

    /**
     * Gets the momentum in x-direction.
     *
     * @return momentum in x-direction.
     **/
    t_real getMomentumX( t_real ,
                         t_real ) const;

    /**
     * Gets the momentum in y-direction.
     * @return momentum in y-direction.
     **/
    t_real getMomentumY( t_real,
                         t_real ) const;
                         
    t_real getBathymetry( t_real i_x,
                          t_real ) const ;

    t_real getBathymetryCsv(t_real i_x) const;
};

#endif