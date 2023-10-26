#include "fwave.h"
#include <cmath>


void tsunami_lab::solvers::fwave::eigenwerte( t_real   i_hL,
                                t_real   i_hR,
                                t_real   i_uL,
                                t_real   i_uR,
                                t_real & o_waveSpeedL,
                                t_real & o_waveSpeedR ){

  t_real l_hSqrtL = std::sqrt( i_hL );
  t_real l_hSqrtR = std::sqrt( i_hR );

  t_real l_hRoe = 0.5f * ( i_hL + i_hR );
  t_real l_uRoe = l_hSqrtL * i_uL + l_hSqrtR * i_uR;
  l_uRoe /= l_hSqrtL + l_hSqrtR;


  t_real l_ghSqrtRoe = m_gSqrt * std::sqrt( l_hRoe );
  o_waveSpeedL = l_uRoe - l_ghSqrtRoe;
  o_waveSpeedR = l_uRoe + l_ghSqrtRoe;
}

void tsunami_lab::solvers::fwave::flux( t_real i_hL,
                                        t_real i_hR,
                                        t_real i_huL,
                                        t_real i_huR,
                                        t_real o_fdelta[2]){
    
    t_real pow2HuL= pow(i_huL, 2)/i_hL;
    t_real gPowHL = g * pow(i_hL, 2);
    t_real gesammtL = pow2HuL + 0.5 * gPowHL;
    t_real fql[2] = {i_huL, gesammtL};

    t_real pow2HuR= pow(i_huR, 2)/i_hR;
    t_real gPowHR = g * pow(i_hR, 2);
    t_real gesammtR = pow2HuR + 0.5 * gPowHR;
    t_real fqr[2] = {i_huR, gesammtR};

    if (o_fdelta == nullptr) {
        o_fdelta = new t_real[2];
    }
    o_fdelta[0] = fqr[0] - fql[0];
    o_fdelta[1] = fqr[1] - fql[1];
}

void tsunami_lab::solvers::fwave::decompose(t_real i_alphas[2],
                                             t_real i_eigens[2],
                                             t_real o_minus_A_deltaQ[2],
                                             t_real o_plus_A_deltaQ[2]){
    o_minus_A_deltaQ[0] = i_alphas[0];
    o_minus_A_deltaQ[1] = i_alphas[0] * i_eigens[0]; 
    
    o_plus_A_deltaQ[0] = i_alphas[1];
    o_plus_A_deltaQ[1] = i_alphas[1] * i_eigens[1]; 
}
void tsunami_lab::solvers::fwave::inverseMatrix(t_real i_eigen1,
                                                t_real i_eigen2,
                                                t_real o_inverse[4]){

    t_real det = i_eigen2 - i_eigen1;
    o_inverse[0] = i_eigen2 / det;
    o_inverse[1] = -1 / det;
    o_inverse[2] = -i_eigen1 / det;
    o_inverse[3] = 1 / det;
    
}

void tsunami_lab::solvers::fwave::eigencoefficientAlpha(t_real i_inverse[4],
                                                        t_real i_spalte[2],
                                                        t_real o_eigencoefficients[2]){
    if (o_eigencoefficients == nullptr) {
        o_eigencoefficients = new t_real[2];
    }
    //m x n ° n x p = 
    o_eigencoefficients[0] = (i_inverse[0] * i_spalte[0]) + (i_inverse[1] * i_spalte[1]);
    o_eigencoefficients[1] = (i_inverse[2] * i_spalte[0]) + (i_inverse[3] * i_spalte[1]);

}

void tsunami_lab::solvers::fwave::netUpdates(t_real   i_hL,
                                             t_real   i_hR,
                                             t_real   i_huL,
                                             t_real   i_huR,
                                             t_real   o_minus_A_deltaQ[2],
                                             t_real   o_plus_A_deltaQ[2]){

    t_real l_uL = i_huL / i_hL;
    t_real l_uR = i_huR / i_hR;
      
    t_real l_sL = 0;
    t_real l_sR = 0;

    eigenwerte(i_hL,i_hR,l_uL,l_uR,l_sL,l_sR);

    t_real l_inverse[4];

    inverseMatrix(l_sL, l_sR, l_inverse);

    t_real l_fdelta[2];
    flux(i_hL,i_hR,i_huL,i_huR,l_fdelta);
    
    t_real l_eigencoefficients[2];
    eigencoefficientAlpha(l_inverse,l_fdelta,l_eigencoefficients);

    t_real l_eigens[2] = {l_sL,l_sR};
    decompose(l_eigencoefficients,l_eigens,o_minus_A_deltaQ,o_plus_A_deltaQ);
}