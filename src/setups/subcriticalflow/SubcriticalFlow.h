/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional dam break problem.
 **/
#ifndef TSUNAMI_LAB_SETUPS_SUBCRITICAL_FLOW_H
#define TSUNAMI_LAB_SETUPS_SUBCRITICAL_FLOW_H

#include "../Setup.h"

namespace tsunami_lab {
  namespace setups {
    class SubcriticalFlow;
  }
}

/**
 * 1d dam break setup.
 **/
class tsunami_lab::setups::SubcriticalFlow: public Setup {
  private:
    //! height on the left side 
    t_real m_heightLeft = 0;
    
    //! height on the right side
    t_real m_heightRight = 0;

    //! location of the dam
    t_real m_locationDam = 0;

    t_real m_b = 0;

  public:
    /**
     * Constructor.
     *
     * @param i_heightLeft water height on the left side of the dam.
     * @param i_heightRight water height on the right side of the dam.
     * @param i_locationDam location (x-coordinate) of the dam.
     **/
    SubcriticalFlow(t_real i_heightLeft,
                    t_real i_heightRight,
                    t_real i_locationDam);

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
};

#endif