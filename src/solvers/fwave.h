#ifndef TSUNAMI_LAB_SOLVERS_FWAVE
#define TSUNAMI_LAB_SOLVERS_FWAVE

#include "../constants.h"

namespace tsunami_lab {
  namespace solvers {
    class fwave;
  }
}


class tsunami_lab::solvers::fwave {
  public:
    static t_real constexpr m_gSqrt = 3.131557121;
    static t_real constexpr g = 9.80665;
    /**
     * @brief Computes the wave speeds.
     *
     * @param i_hL height of the left side.
     * @param i_hR height of the right side.
     * @param i_uL particle velocity of the leftside.
     * @param i_uR particles velocity of the right side.
     * @param o_waveSpeedL will be set to the speed of the wave propagating to the left.
     * @param o_waveSpeedR will be set to the speed of the wave propagating to the right.
     **/
    static void eigenwerte( t_real   i_hL,
                            t_real   i_hR,
                            t_real   i_uL,
                            t_real   i_uR,
                            t_real & o_waveSpeedL,
                            t_real & o_waveSpeedR );

    /**
     * @brief Computes Δf.
     *
     * @param i_hL height of the left side.
     * @param i_hR height of the right side.
     * @param i_uL particle velocity of the leftside.
     * @param i_uR particles velocity of the right side.
     * @param o_fdelta compute Δf : jump in fluxes
     **/
    static void flux( t_real i_hL,
                      t_real i_hR,
                      t_real i_huL,
                      t_real i_huR,
                      t_real o_fdelta[2]);

    /**
     * @brief Computes speed of wave propagation (o_minus/plus_A_deltaQ).
     * @param i_alphas the eigencoefficients
     * @param i_eigens array of eigenvalues but used as vectors {1,eigen1} and {1,eigen2}
     * @param o_minus_A_deltaQ will be set to the net-updates for the left side; 0: height, 1: momentum.
     * @param o_plus_A_deltaQ will be set to the net-updates for the right side; 0: height, 1: momentum. 
    */
    static void decompose(t_real i_alphas[2],
                          t_real i_eigens[2],
                          t_real o_minus_A_deltaQ[2],
                          t_real o_plus_A_deltaQ[2]);
    /**
     * @brief Computes the inverse matrix for the eigencoefficients α
     * @param i_eigens1 eigenvalue 1
     * @param i_eigens2 eigenvalue 2
     * @param o_inverse inverse 2 x 2 matrix
    */
    static void inverseMatrix(t_real i_eigen1,
                              t_real i_eigen2,
                              t_real o_inverse[4]);

    /**
     * @brief Computes the eigencoefficients α
     * @param i_inverse inverse 2 x 2 matrix
     * @param i_delta_f jump in fluxes
     * @param o_eigencoefficients the eigencoefficients α_1 and α_1
    */
    static void eigencoefficientAlpha(t_real i_inverse[4],
                                      t_real i_delta_f[2],
                                      t_real o_eigencoefficients[2]);
  
  //public:
  /**
     * @brief Computes both deltaQ.
     *
     * @param i_hL height of the left side.
     * @param i_hR height of the right side.
     * @param i_uL particle velocity of the leftside.
     * @param i_uR particles velocity of the right side.
     * @param o_minus_A_deltaQ will be set to the net-updates for the left side; 0: height, 1: momentum.
     * @param o_plus_A_deltaQ  will be set to the net-updates for the right side; 0: height, 1: momentum. 
     **/
    static void netUpdates( t_real   i_hL,
                            t_real   i_hR,
                            t_real   i_huL,
                            t_real   i_huR,
                            t_real   o_minus_A_deltaQ[2],
                            t_real   o_plus_A_deltaQ[2] );                               
};

#endif