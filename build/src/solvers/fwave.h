#ifndef TSUNAMI_LAB_SOLVERS_FWAVE
#define TSUNAMI_LAB_SOLVERS_FWAVE

#include "../constants.h"

namespace tsunami_lab {
  namespace solvers {
    class fwave;
  }
}

class tsunami_lab::solvers::fwave {
  private:
    static t_real constexpr m_gSqrt = 3.131557121;
    static t_real constexpr g = 9.80665;

    static void eigenwerte( t_real   i_hL,
                            t_real   i_hR,
                            t_real   i_uL,
                            t_real   i_uR,
                            t_real & o_waveSpeedL,
                            t_real & o_waveSpeedR );

    static void flux( t_real i_hL,
                      t_real i_hR,
                      t_real i_huL,
                      t_real i_huR,
                      t_real o_fdelta[2]);

    static void decompose(t_real i_alphas[2],
                          t_real i_eigens[2],
                          t_real o_minus_A_deltaQ[2],
                          t_real o_plus_A_deltaQ[2]);

    static void inverseMatrix(t_real i_eigen1,
                              t_real i_eigen2,
                              t_real o_inverse[4]);

    static void eigencoefficientAlpha(t_real i_inverse[4],
                                      t_real i_spalte[2],
                                      t_real o_eigencoefficients[2]);
  
  public:
    static void netUpdates( t_real   i_hL,
                            t_real   i_hR,
                            t_real   i_huL,
                            t_real   i_huR,
                            t_real   o_minus_A_deltaQ[2],
                            t_real   o_plus_A_deltaQ[2] );                               
};

#endif