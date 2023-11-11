/**
 * @author Ward Tammaa 
 *
 * @section DESCRIPTION
 * subcriticalFlow.
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
 * SubcriticalFlow.
 **/
class tsunami_lab::setups::SubcriticalFlow: public Setup {

  public:

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