/**
 * @author Ward Tammaa 
 *
 * @section DESCRIPTION

 **/
#ifndef TSUNAMI_LAB_SETUPS_TSUNAMIEVENT2D_H
#define TSUNAMI_LAB_SETUPS_TSUNAMIEVENT2D_H

#include "../Setup.h"
#include <fstream>
#include <sstream>
#include <vector>


namespace tsunami_lab {
  namespace setups {
    class TsunamiEvent2d;
  }
}

/**
 * TsunamiEvent2d setup.
 **/
class tsunami_lab::setups::TsunamiEvent2d: public Setup {

  private:
    /**
     * @param m_delta avoids running into numerical issues due to missing support for wetting and drying in our solver.
     */
    t_real m_delta               = 0;
    t_real m_width_bathymetry    = 0; 
    t_real m_length_bathymetry   = 0;
    t_real m_width_displacement  = 0;
    t_real m_length_displacement = 0;


    /**
    * @param m_bathymetry_values bathymetry values without ghostcells.
    */
    std::vector<t_real> m_bathymetry_values;
    /**
    * @param m_bathymetry_x_values 1D array for the bathymetry x values.
    */
    std::vector<t_real> m_bathymetry_x_values;
    /**
    * @param m_bathymetry_y_values 1D array for the bathymetry y values.
    */
    std::vector<t_real> m_bathymetry_y_values;



    /**
    * @param m_displacement_values bathymetry values without ghostcells.
    */
    std::vector<t_real> m_displacement_values;
    /**
    * @param m_displacement_x_values 1D array for the bathymetry x values.
    */
    std::vector<t_real> m_displacement_x_values;
    /**
    * @param m_displacement_y_values 1D array for the bathymetry y values.
    */
    std::vector<t_real> m_displacement_y_values;

    /**
     * @brief check if position is in boundary true : call findClosestIndex else : 0; 
     *   
     * @param i_x xPos in the netcdf file.
     * @param i_y YPos in the netcdf file.
     * @return bathymetry value.
     */

    t_real getBathymetryNetCdf(t_real i_x, t_real i_y) const;
    
    /**
     * @param vec vector to find the closest available value in the vector if the exact coordinates are not available.
     * @param value queries the value at point.
     * @return index of the closest available value.
     */
    t_idx findClosestIndex(const std::vector<t_real>& vec, t_real value) const;

  public:
   
    /**
     * @brief check if position is in boundary true : call findClosestIndex else : 0; 
     * 
     * @param i_x xPos in the netcdf file.
     * @param i_y YPosin the netcdf file.
     * @return displacement value.
     */
   
    t_real displacement( t_real i_x,t_real i_y) const;
    /**
     * @brief The constructor for TsunamiEvent2d.
     * @param i_delta to avoid running into numerical issues (small value).
     **/

    TsunamiEvent2d( t_real i_delta);

    /**
     * @brief Gets the water height at a given point.
     * @param i_x xPos in the netcdf file.
     * @param i_y yPos in the netcdf file.
     * @return Height at the given point.
     **/
    t_real getHeight( t_real i_x,
                      t_real i_y) const;

    /**
     * @brief Gets the momentum in x-direction.
     *
     * @return Momentum in x-direction.
     **/
    t_real getMomentumX( t_real ,
                         t_real ) const;

    /**
     * @brief Gets the momentum in y-direction.
     * @return Momentum in y-direction.
     **/
    t_real getMomentumY( t_real,
                         t_real ) const;
    
    /**
    * @brief Computes the bathymetry as explained in 5.2.
    * @param i_x xPos in the netcdf file.
    * @param i_y xPos in the netcdf file.
    * @return Bathymetry.
    */
    t_real getBathymetry( t_real i_x,
                          t_real i_y) const ;


};

#endif