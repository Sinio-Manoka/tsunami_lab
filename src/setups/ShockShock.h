/**
 * @author Ward Tammaa 
 *
 * @section DESCRIPTION
 * The Shock Shock problem.
 **/
 
#ifndef TSUNAMI_LAB_SETUPS_SHOCK_SHOCK_H
#define TSUNAMI_LAB_SETUPS_SHOCK_SHOCK_H

#include "Setup.h"

namespace tsunami_lab {
  namespace setups {
    class ShockShock;
  }
}

/**
 * Shock Shock setup.
 **/
class tsunami_lab::setups::ShockShock: public Setup {
  private:
    //! height 
    t_real m_height = 0;

    //! impulse 
    t_real m_hu = 0;

    //! location
    t_real m_locationDam = 0;

  public:

    /**
     * Constructor.
     *
     * @param i_height water height 
     * @param i_hu water impulse 
     * @param i_locationDam location (x-coordinate) .    
     **/

    ShockShock( t_real i_height,
                t_real i_hu,
                t_real i_locationDam);

    /**
     * Gets the water height at a given point.
     *
     * @return height at the given point.
     **/
    t_real getHeight( t_real,
                      t_real) const;


    /**
     * Gets the momentum in x-direction.
     * @param i_x x-coordinate of the queried point.
     * @return momentum in x-direction.
     **/
    t_real getMomentumX( t_real i_x,
                         t_real ) const;

    /**
     * Gets the momentum in y-direction.
     * @return momentum in y-direction.
     **/
    t_real getMomentumY( t_real,
                         t_real ) const;

};

#endif