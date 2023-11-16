/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional wave propagation patch.
 **/
#ifndef TSUNAMI_LAB_PATCHES_WAVE_PROPAGATION_2D
#define TSUNAMI_LAB_PATCHES_WAVE_PROPAGATION_2D

#include "../WavePropagation.h"

namespace tsunami_lab {
  namespace patches {
    class WavePropagation2d;
  }
}

class tsunami_lab::patches::WavePropagation2d: public WavePropagation {
  private:
    //! current step which indicates the active values in the arrays below
    unsigned short m_step = 0;

    //! boolean to which solver will be used
    bool m_choice = true;
    //if true then we'll use reflecting boundary conditions at cell m_nCells+1 (last cell)
    bool m_choiceBoundry = false;

    //! number of cells discretizing the computational domain
    t_idx m_nCells = 0;
    t_real * m_b = nullptr;
    //! water heights for the current and next time step for all cells
    t_real * m_h[2] = { nullptr, nullptr };

    //! momenta for the current and next time step for all cells
    t_real * m_hu[2] = { nullptr, nullptr };
    t_real * m_hv[2] = { nullptr, nullptr };

    
    /**
     * @brief Get the Index object
     * 
     * @param i_ix 
     * @param i_iy 
     * @return tsunami_lab::t_idx 
     */
    tsunami_lab::t_idx getIndex(tsunami_lab::t_idx  i_ix,tsunami_lab::t_idx  i_iy){
      return (m_nCells+2) * i_iy +i_ix;
    }
    /*
     * @brief Set the GhostoutFlowfor Edges 
     *  void setGhostoutFlowforEdges();
     */
   


  public:
    /**
     * @brief Constructs the 1d wave propagation solver.
     * @param i_choice which solver to choice from (true means Roe and false means our Fwave).
     * @param i_nCells number of cells.
     **/
    WavePropagation2d( t_idx i_nCellsm, bool i_choice );

    /**
     * @brief Destructor which frees all allocated memory.
     **/
    ~WavePropagation2d();

    /**
     * @brief Performs a time step.
     *
     * @param i_scaling scaling of the time step (dt / dx).
     **/
    void timeStep( t_real i_scaling );

    /**
     * @brief Sets the values of the ghost cells according to outflow boundary conditions.
     **/
    void setGhostOutflow(bool m_choiceBoundry);

    /**
     * @brief Gets the stride in y-direction. x-direction is stride-1.
     *
     * @return stride in y-direction.
     **/
    t_idx getStride(){
      return m_nCells+2;
    }

    /**
     * @brief Gets the cells water heights.
     *
     * @return Water heights.
     */
    t_real const * getHeight(){
      return m_h[m_step];
    }

    /**
     * @brief Gets cells' Bathymetry.
     *
     * @return Bathymetry.
     */
   t_real const * getBathymetry(){
      return m_b;
    }

    /**
     * @brief Gets the cells' momenta in x-direction.
     *
     * @return momenta in x-direction.
     **/
    t_real const * getMomentumX(){
      return m_hu[m_step];
    }

    /**
     * @brief Dummy function which returns a nullptr.
     **/
    t_real const * getMomentumY(){
      return m_hv[m_step];
    }

    /**
     * @brief Sets the height of the cell to the given value.
     * @param i_ix id of the cell in x-direction.
     * @param i_ix id of the cell in x-direction.
     * @param i_h water height.
     **/
    void setHeight( t_idx  i_ix,
                    t_idx  i_iy,
                    t_real i_h ) {
      m_h[m_step][getIndex(i_ix+1,i_iy+1)] = i_h;
    }


    /**
     * @brief Sets the Bathymetry of the cell to the given value.
     *
     * @param i_ix id of the cell in x-direction.
     * @param i_b Bathymetry.
     **/
    void setBathymetry(t_idx  i_ix,
                       t_idx  i_iy,
                       t_real i_b){
      m_b[getIndex(i_ix+1,i_iy+1)] = i_b;
    }

    /**
     * @brief Sets the momentum in x-direction to the given value.
     *
     * @param i_ix id of the cell in x-direction.
     * @param i_hu momentum in x-direction.
     **/
    void setMomentumX( t_idx  i_ix,
                       t_idx  i_iy,
                       t_real i_hu ) {
      m_hu[m_step][getIndex(i_ix+1,i_iy+1)] = i_hu;
    }

    /**
     * @brief Dummy function since there is no y-momentum in the 1d solver.
     **/
    void setMomentumY( t_idx  i_ix,
                       t_idx  i_iy,
                       t_real i_hv) {
      m_hv[m_step][getIndex(i_ix+1,i_iy+1)] = i_hv;
    }
    

    

};

#endif