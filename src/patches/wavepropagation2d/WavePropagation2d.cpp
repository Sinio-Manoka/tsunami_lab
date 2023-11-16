/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * One-dimensional wave propagation patch.
 **/
#include "WavePropagation2d.h"
#include "../../solvers/Roe.h"
#include "../../solvers/fwave.h"



tsunami_lab::patches::WavePropagation2d::WavePropagation2d( t_idx i_nCells, bool i_choice ) {
  m_choice = i_choice;
  m_nCells = i_nCells;

  // allocate memory including a single ghost cell on each side
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    m_h[l_st]  = new t_real[ (m_nCells+2) * (m_nCells+2) ];
    m_hu[l_st] = new t_real[ (m_nCells+2) * (m_nCells+2) ];
    m_hv[l_st] = new t_real[ (m_nCells+2) * (m_nCells+2) ];
  }
  m_b = new t_real[(m_nCells+2) * (m_nCells+2)];

  // init to zero
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    for( t_idx l_ce = 0; l_ce <  (m_nCells+2) * (m_nCells+2) ; l_ce++ ) {
      m_h[l_st][l_ce] = 0;
      m_hu[l_st][l_ce] = 0;
      m_hv[l_st][l_ce] = 0;
      m_b[l_ce] = 0;
    }
  }
}
 //free memory
tsunami_lab::patches::WavePropagation2d::~WavePropagation2d() {
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    delete[] m_h[l_st];
    delete[] m_hu[l_st];
    delete[] m_hv[l_st];
  }
  delete[] m_b;
}

void tsunami_lab::patches::WavePropagation2d::timeStep( t_real i_scaling) {
  // pointers to old and new data
  t_real * l_hOld  = m_h[m_step];
  t_real * l_huOld = m_hu[m_step];
  t_real * l_hvOld = m_hv[m_step];

  t_real * l_b  = m_b; 
  m_step = (m_step+1) % 2;
  t_real * l_hNew =  m_h[m_step];
  t_real * l_huNew = m_hu[m_step];
  t_real * l_hvNew = m_hv[m_step];


  for( t_idx l_ce = 1; l_ce < ((m_nCells+2) * (m_nCells+2)); l_ce++ ) {
    l_hNew[l_ce]  = l_hOld[l_ce];
    l_huNew[l_ce] = l_huOld[l_ce];
    l_hvNew[l_ce] = l_hvOld[l_ce];
  }
  setGhostOutflow(true);
  
  for(t_idx l_ex = 0; l_ex < m_nCells +1;l_ex++){ 
    for(t_idx l_ey = 0; l_ey < m_nCells +1;l_ey++){
      t_real l_netUpdates[2][2];
      t_idx l_ceL = getIndex(l_ey,l_ex);
      t_idx l_ceR = getIndex(l_ey+1,l_ex);
      if(m_choice){
        solvers::Roe::netUpdates(l_hOld[l_ceL],
                                l_hOld[l_ceR],
                                l_huOld[l_ceL],
                                l_huOld[l_ceR],
                                l_netUpdates[0],
                                l_netUpdates[1]);
      }else{
        solvers::fwave::netUpdates( l_hOld[l_ceL],
                                    l_hOld[l_ceR],
                                    l_huOld[l_ceL],
                                    l_huOld[l_ceR],
                                    l_b[l_ceL],
                                    l_b[l_ceR],
                                    l_netUpdates[0],
                                    l_netUpdates[1]);
      }
      l_hNew[l_ceL]  -= i_scaling * l_netUpdates[0][0];
      l_huNew[l_ceL] -= i_scaling * l_netUpdates[0][1];
      l_hNew[l_ceR]  -= i_scaling * l_netUpdates[1][0];
      l_huNew[l_ceR] -= i_scaling * l_netUpdates[1][1];
      
    }

  }
  l_hOld  = m_h[m_step];
  l_huOld = m_hu[m_step];
  l_hvOld = m_hv[m_step];
  m_step = (m_step+1) % 2;
  l_hNew =  m_h[m_step];
  l_huNew = m_hu[m_step];
  l_hvNew = m_hv[m_step];

  for( t_idx l_ce = 0; l_ce < ((m_nCells+2) * (m_nCells+2)); l_ce++ ) {
    l_hNew[l_ce]  = l_hOld[l_ce];
    l_huNew[l_ce] = l_huOld[l_ce];
    l_hvNew[l_ce] = l_hvOld[l_ce];
  }
  setGhostOutflow(true);

  for(t_idx l_ex = 0; l_ex < m_nCells +1;l_ex++){
    for(t_idx l_ey = 0; l_ey < m_nCells +1;l_ey++){
      t_real l_netUpdates[2][2];

      t_idx l_ceL = getIndex(l_ey,l_ex);
      t_idx l_ceR = getIndex(l_ey,l_ex+1);
      
      if(m_choice){
        solvers::Roe::netUpdates( l_hOld[l_ceL],
                                  l_hOld[l_ceR],
                                  l_hvOld[l_ceL],
                                  l_hvOld[l_ceR],
                                  l_netUpdates[0],
                                  l_netUpdates[1]);
      }else{
        solvers::fwave::netUpdates( l_hOld[l_ceL],
                                    l_hOld[l_ceR],
                                    l_hvOld[l_ceL],
                                    l_hvOld[l_ceR],
                                    l_b[l_ceL],
                                    l_b[l_ceR],
                                    l_netUpdates[0],
                                    l_netUpdates[1]);
      }
      l_hNew[l_ceL]  -= i_scaling * l_netUpdates[0][0];
      l_hvNew[l_ceL] -= i_scaling * l_netUpdates[0][1];
      l_hNew[l_ceR]  -= i_scaling * l_netUpdates[1][0];
      l_hvNew[l_ceR] -= i_scaling * l_netUpdates[1][1];
      
    }
  }
}

void tsunami_lab::patches::WavePropagation2d::setGhostOutflow(bool i_choiceBoundry) {
  m_choiceBoundry = i_choiceBoundry;
  t_real * l_h = m_h[m_step];
  t_real * l_hu = m_hu[m_step];
  t_real * l_hv = m_hv[m_step];
  t_real * l_b = m_b;
    // bottom row & top row
    for (t_idx l_g = 1; l_g < m_nCells+1; l_g++)
    {
      l_h[l_g] = l_h[getIndex(l_g,1)];
      l_h[getIndex(l_g,m_nCells+1)] = l_h[getIndex(l_g,m_nCells)];
    //(m_nCells+2) * i_iy +i_ix; = 
      if(i_choiceBoundry)
      {
        l_hu[l_g] = -l_hu[getIndex(l_g,1)];
        l_hu[getIndex(l_g,m_nCells+1)] = -l_hu[getIndex(l_g,m_nCells)];
        l_hv[l_g] = -l_hv[getIndex(l_g,1)];
        l_hv[getIndex(l_g,m_nCells+1)] = -l_hv[getIndex(l_g,m_nCells)];
      }
      else
      {
        l_hu[l_g] = l_hu[getIndex(l_g,1)];
        l_hu[getIndex(l_g,m_nCells+1)] = l_hu[getIndex(l_g,m_nCells)];
        l_hv[l_g] = l_hv[getIndex(l_g,1)];
        l_hv[getIndex(l_g,m_nCells+1)] = l_hv[getIndex(l_g,m_nCells)];
      }

      l_b[l_g] = l_b[getIndex(l_g,1)];
      l_b[getIndex(l_g,m_nCells+1)] = l_b[getIndex(l_g,m_nCells)];
    }
    
    // leftest and rightest column
    for (t_idx l_g = 1; l_g <m_nCells+1; l_g++)
    {
      l_h[getIndex(0,l_g)] = l_h[getIndex(1,l_g)];
      l_h[getIndex(m_nCells+1,l_g)] = l_h[getIndex(m_nCells,l_g)];
      if(i_choiceBoundry)
      {
        l_hu[getIndex(0,l_g)] = -l_hu[getIndex(1,l_g)];
        l_hu[getIndex(m_nCells+1,l_g)] = -l_hu[getIndex(m_nCells,l_g)];

        l_hv[getIndex(0,l_g)] = -l_hv[getIndex(1,l_g)];
        l_hv[getIndex(m_nCells+1,l_g)] = -l_hv[getIndex(m_nCells,l_g)];
      }
      else
      {
        l_hu[getIndex(0,l_g)] = l_hu[getIndex(1,l_g)];
        l_hu[getIndex(m_nCells+1,l_g)] = l_hu[getIndex(m_nCells,l_g)];
        l_hv[getIndex(0,l_g)] = l_hv[getIndex(1,l_g)];
        l_hv[getIndex(m_nCells+1,l_g)] = l_hv[getIndex(m_nCells,l_g)];
      }
      l_b[getIndex(0,l_g)] = l_b[getIndex(1,l_g)];
      l_b[getIndex(m_nCells+1,l_g)] = l_b[getIndex(m_nCells,l_g)];
    }
      // Ecken des Gitters aktualisieren

      l_b[getIndex(0,0)] = l_b[getIndex(1,1)];
      l_b[getIndex(m_nCells+1,0)] = l_b[getIndex(m_nCells,1)];
      l_b[getIndex(0,m_nCells+1)] = l_b[getIndex(1,m_nCells)];
      l_b[getIndex(m_nCells+1,m_nCells+1)] = l_b[getIndex(m_nCells,m_nCells)];
      
      l_h[getIndex(0,0)] = l_h[getIndex(1,1)];
      l_h[getIndex(m_nCells+1,0)] = l_h[getIndex(m_nCells,1)];
      l_h[getIndex(0,m_nCells+1)] = l_h[getIndex(1,m_nCells)];
      l_h[getIndex(m_nCells+1,m_nCells+1)] = l_h[getIndex(m_nCells,m_nCells)];

      l_hu[getIndex(0,0)] = l_hu[getIndex(1,1)];
      l_hu[getIndex(m_nCells+1,0)] = l_hu[getIndex(m_nCells,1)];
      l_hu[getIndex(0,m_nCells+1)] = l_hu[getIndex(1,m_nCells)];
      l_hu[getIndex(m_nCells+1,m_nCells+1)] = l_hu[getIndex(m_nCells,m_nCells)];

      l_hv[getIndex(0,0)] = l_hv[getIndex(1,1)];
      l_hv[getIndex(m_nCells+1,0)] = l_hv[getIndex(m_nCells,1)];
      l_hv[getIndex(0,m_nCells+1)] = l_hv[getIndex(1,m_nCells)];
      l_hv[getIndex(m_nCells+1,m_nCells+1)] = l_hv[getIndex(m_nCells,m_nCells)];
}