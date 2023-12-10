/**
 * @author Ward Tammaa, Mohamad khaled Minawe
 * @section DESCRIPTION
 * tow-dimensional wave propagation patch.
 **/
#include "WavePropagation2d.h"
#include "../../solvers/Roe.h"
#include "../../solvers/fwave.h"
#include <iostream>



tsunami_lab::patches::WavePropagation2d::WavePropagation2d( t_idx i_xCells,t_idx i_yCells, bool i_choice ) {
  m_choice = i_choice;
  m_xCells = i_xCells; // anzahl der spalten
  m_yCells = i_yCells; // anzahl der zeilen

  // allocate memory including a single ghost cell on each side
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    m_h[l_st]  = new t_real[ (m_xCells+2) * (m_yCells+2) ]{};
    m_hu[l_st] = new t_real[ (m_xCells+2) * (m_yCells+2) ]{};
    m_hv[l_st] = new t_real[ (m_xCells+2) * (m_yCells+2) ]{};
  }
  m_b = new t_real[(m_xCells+2) * (m_yCells+2)]{};

  // init to zero
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    for( t_idx l_ce = 0; l_ce <  (m_xCells+2) * (m_yCells+2) ; l_ce++ ) {
      m_h[l_st][l_ce] = 0;
      m_hu[l_st][l_ce] = 0;
      m_hv[l_st][l_ce] = 0;
      if(l_st==0){
        m_b[l_ce] = 0;
      }
    }
  }
}
 //free memory
tsunami_lab::patches::WavePropagation2d::~WavePropagation2d() {
  delete[] m_b;
  for( unsigned short l_st = 0; l_st < 2; l_st++ ) {
    delete[] m_hv[l_st];
    delete[] m_h[l_st];
    delete[] m_hu[l_st];
  }
}

void tsunami_lab::patches::WavePropagation2d::timeStep( t_real i_scaling) {
  // pointers to old and new data
  t_real * l_hOld  = m_h[m_step]; //0
  t_real * l_huOld = m_hu[m_step];
  t_real * l_hvOld = m_hv[m_step];

  t_real * l_b  = m_b; 
  m_step = (m_step+1) % 2;
  t_real * l_hNew =  m_h[m_step]; //1
  t_real * l_huNew = m_hu[m_step];
  t_real * l_hvNew = m_hv[m_step];


  for( t_idx l_ce = 1; l_ce < ((m_xCells+2) * (m_yCells+2)); l_ce++ ) {
    l_hNew[l_ce]  = l_hOld[l_ce];
    l_huNew[l_ce] = l_huOld[l_ce];
    l_hvNew[l_ce] = l_hvOld[l_ce];
  }
  setGhostOutflow(false);
  
 for(t_idx l_ey = 1; l_ey < m_yCells +1;l_ey++){ 
    for(t_idx l_ex = 1; l_ex < m_xCells +1;l_ex++){
      t_real l_netUpdates[2][2];
      t_idx l_ceL = getIndex(l_ex,l_ey);
      t_idx l_ceR = getIndex(l_ex+1,l_ey);

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
  l_hOld  = m_h[m_step]; //l_hOld zeigt nun auf l_hNew
  l_huOld = m_hu[m_step];
  l_hvOld = m_hv[m_step];
  m_step = (m_step+1) % 2;
  l_hNew =  m_h[m_step];
  l_huNew = m_hu[m_step]; //l_huNew zeigt nun auf die alten werte
  l_hvNew = m_hv[m_step];

  for( t_idx l_ce = 0; l_ce < ((m_xCells+2) * (m_yCells+2)); l_ce++ ) {
    l_hNew[l_ce]  = l_hOld[l_ce];
    l_huNew[l_ce] = l_huOld[l_ce];
    l_hvNew[l_ce] = l_hvOld[l_ce];
  } //da l_hold nun auf die neuen werte zeigt werden die neuen in in hnew geschrieben in den sich die alten werte befinden
  setGhostOutflow(false);

  for(t_idx l_ex = 1; l_ex < m_xCells +1;l_ex++){
    for(t_idx l_ey = 1; l_ey < m_yCells +1;l_ey++){
      t_real l_netUpdates[2][2];

      t_idx l_ceL = getIndex(l_ex,l_ey);
      t_idx l_ceR = getIndex(l_ex,l_ey+1);
      
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
    }//die berechnungen werden in l_New geschrieben
  }
}

void tsunami_lab::patches::WavePropagation2d::setGhostOutflow(bool i_choiceBoundry) {
  m_choiceBoundry = i_choiceBoundry;
  t_real * l_h = m_h[m_step];
  t_real * l_hu = m_hu[m_step];
  t_real * l_hv = m_hv[m_step];
  t_real * l_b = m_b;
    // bottom row & top row
    for (t_idx l_g = 1; l_g < m_xCells+1; l_g++)
    {
      l_h[l_g] = l_h[getIndex(l_g,1)];
      l_h[getIndex(l_g,m_yCells+1)] = l_h[getIndex(l_g,m_yCells)]; 
      if(i_choiceBoundry)
      {
        l_hu[l_g] = -l_hu[getIndex(l_g,1)];
        l_hu[getIndex(l_g,m_yCells+1)] = -l_hu[getIndex(l_g,m_yCells)];
        l_hv[l_g] = -l_hv[getIndex(l_g,1)];
        l_hv[getIndex(l_g,m_yCells+1)] = -l_hv[getIndex(l_g,m_yCells)];
      }
      else
      {
        l_hu[l_g] = l_hu[getIndex(l_g,1)];
        l_hu[getIndex(l_g,m_yCells+1)] = l_hu[getIndex(l_g,m_yCells)];
        l_hv[l_g] = l_hv[getIndex(l_g,1)];
        l_hv[getIndex(l_g,m_yCells+1)] = l_hv[getIndex(l_g,m_yCells)];
      }

      l_b[l_g] = l_b[getIndex(l_g,1)];
      l_b[getIndex(l_g,m_yCells+1)] = l_b[getIndex(l_g,m_yCells)];
    }
    
    // leftest and rightest column
    for (t_idx l_g = 1; l_g <m_yCells+1; l_g++)
    {
      l_h[getIndex(0,l_g)] = l_h[getIndex(1,l_g)];
      l_h[getIndex(m_xCells+1,l_g)] = l_h[getIndex(m_xCells,l_g)];
      if(i_choiceBoundry)
      {
        l_hu[getIndex(0,l_g)] = -l_hu[getIndex(1,l_g)];
        l_hu[getIndex(m_xCells+1,l_g)] = -l_hu[getIndex(m_xCells,l_g)];

        l_hv[getIndex(0,l_g)] = -l_hv[getIndex(1,l_g)];
        l_hv[getIndex(m_xCells+1,l_g)] = -l_hv[getIndex(m_xCells,l_g)];
      }
      else
      {
        l_hu[getIndex(0,l_g)] = l_hu[getIndex(1,l_g)];
        l_hu[getIndex(m_xCells+1,l_g)] = l_hu[getIndex(m_xCells,l_g)];
        l_hv[getIndex(0,l_g)] = l_hv[getIndex(1,l_g)];
        l_hv[getIndex(m_xCells+1,l_g)] = l_hv[getIndex(m_xCells,l_g)];
      }
      l_b[getIndex(0,l_g)] = l_b[getIndex(1,l_g)];
      l_b[getIndex(m_xCells+1,l_g)] = l_b[getIndex(m_xCells,l_g)];
    }
      // Ecken des Gitters aktualisieren

      l_b[getIndex(0,0)] = l_b[getIndex(1,1)];
      l_h[getIndex(0,0)] = l_h[getIndex(1,1)];
      l_hu[getIndex(0,0)] = l_hu[getIndex(1,1)];
      l_hv[getIndex(0,0)] = l_hv[getIndex(1,1)];

      l_b[getIndex(m_xCells+1,0)] = l_b[getIndex(m_xCells,1)];
      l_h[getIndex(m_xCells+1,0)] = l_h[getIndex(m_xCells,1)];
      l_hu[getIndex(m_xCells+1,0)] = l_hu[getIndex(m_xCells,1)];
      l_hv[getIndex(m_xCells+1,0)] = l_hv[getIndex(m_xCells,1)];
      
      l_b[getIndex(0,m_yCells+1)] = l_b[getIndex(1,m_yCells)];
      l_h[getIndex(0,m_yCells+1)] = l_h[getIndex(1,m_yCells)];
      l_hu[getIndex(0,m_yCells+1)] = l_hu[getIndex(1,m_yCells)];
      l_hv[getIndex(0,m_yCells+1)] = l_hv[getIndex(1,m_yCells)];
      
      l_b[getIndex(m_xCells+1,m_yCells+1)] = l_b[getIndex(m_xCells,m_yCells)];
      l_h[getIndex(m_xCells+1,m_yCells+1)] = l_h[getIndex(m_xCells,m_yCells)];
      l_hu[getIndex(m_xCells+1,m_yCells+1)] = l_hu[getIndex(m_xCells,m_yCells)];
      l_hv[getIndex(m_xCells+1,m_yCells+1)] = l_hv[getIndex(m_xCells,m_yCells)];
}