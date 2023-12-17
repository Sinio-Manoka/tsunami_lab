/**
 * @author Ward Tammaa, Mohamad khaled Minawe
 * @section DESCRIPTION
 * tow-dimensional wave propagation patch.
 **/
#include "WavePropagation2d.h"
#include "../../solvers/Roe.h"
#include "../../solvers/fwave.h"
#include <iostream>

tsunami_lab::patches::WavePropagation2d::WavePropagation2d(t_idx i_xCells, t_idx i_yCells, bool i_choice, bool i_choiceBoundary)
{
  m_choice = i_choice; // solver choice
  m_choiceBoundry = i_choiceBoundary; // Ghostzellverwaltung
  m_xCells = i_xCells; // anzahl der spalten
  m_yCells = i_yCells; // anzahl der zeilen

  // allocate memory including a single ghost cell on each side

  m_h = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
  m_hu = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
  m_hv = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
  m_b = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
  // again first pointer for height and second for momentum
  m_h_uv[0] = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
  m_h_uv[1] = new t_real[(m_xCells + 2) * (m_yCells + 2)]{};
}
// free memory
tsunami_lab::patches::WavePropagation2d::~WavePropagation2d()
{

  delete[] m_b;
  delete[] m_hv;
  delete[] m_h;
  delete[] m_hu;
  delete[] m_h_uv[0];
  delete[] m_h_uv[1];
}

void tsunami_lab::patches::WavePropagation2d::timeStep(t_real i_scaling)
{
  
  t_real * l_temp_h = m_h_uv[0];
  t_real * l_temp_huv = m_h_uv[1];

  for (t_idx l_ce = 1; l_ce < ((m_xCells + 2) * (m_yCells + 2)); l_ce++)
  { // zuerst hu
    l_temp_h[l_ce] = m_h[l_ce];
    l_temp_huv[l_ce] = m_hu[l_ce];
  }
  setGhostCollumn();

  for (t_idx l_ex = 1; l_ex < m_xCells + 1; l_ex++)
  {
    for (t_idx l_ey = 1; l_ey < m_yCells + 1; l_ey++)
    {
      t_real l_netUpdates[2][2];
      t_idx l_ceL = getIndex(l_ex, l_ey);
      t_idx l_ceR = getIndex(l_ex + 1, l_ey);

      if (m_choice)
      { 
        solvers::Roe::netUpdates(l_temp_h[l_ceL],
                                 l_temp_h[l_ceR],
                                 l_temp_huv[l_ceL],
                                 l_temp_huv[l_ceR],
                                 l_netUpdates[0],
                                 l_netUpdates[1]);
      }
      else
      {
        solvers::fwave::netUpdates(l_temp_h[l_ceL],
                                   l_temp_h[l_ceR],
                                   l_temp_huv[l_ceL],
                                   l_temp_huv[l_ceR],
                                   m_b[l_ceL],
                                   m_b[l_ceR],
                                   l_netUpdates[0],
                                   l_netUpdates[1]);
      }
      m_h[l_ceL] -= i_scaling * l_netUpdates[0][0];
      m_hu[l_ceL] -= i_scaling * l_netUpdates[0][1];
      m_h[l_ceR] -= i_scaling * l_netUpdates[1][0];
      m_hu[l_ceR] -= i_scaling * l_netUpdates[1][1];

    } 
  }

  for (t_idx l_ce = 0; l_ce < ((m_xCells + 2) * (m_yCells + 2)); l_ce++)
  { //jetzt
    l_temp_h[l_ce] = m_h[l_ce];
    l_temp_huv[l_ce] = m_hv[l_ce];
  } 
  setGhostRow();

  for (t_idx l_ex = 1; l_ex < m_xCells + 1; l_ex++)
  {
    for (t_idx l_ey = 1; l_ey < m_yCells + 1; l_ey++)
    {
      t_real l_netUpdates[2][2];

      t_idx l_ceL = getIndex(l_ex, l_ey);
      t_idx l_ceR = getIndex(l_ex, l_ey + 1);

      if (m_choice)
      {
        solvers::Roe::netUpdates(l_temp_h[l_ceL],
                                 l_temp_h[l_ceR],
                                 l_temp_huv[l_ceL],
                                 l_temp_huv[l_ceR],
                                 l_netUpdates[0],
                                 l_netUpdates[1]);
      }
      else
      {
        solvers::fwave::netUpdates(l_temp_h[l_ceL],
                                   l_temp_h[l_ceR],
                                   l_temp_huv[l_ceL],
                                   l_temp_huv[l_ceR],
                                   m_b[l_ceL],
                                   m_b[l_ceR],
                                   l_netUpdates[0],
                                   l_netUpdates[1]);
      }
      m_h[l_ceL] -= i_scaling * l_netUpdates[0][0];
      m_hv[l_ceL] -= i_scaling * l_netUpdates[0][1];
      m_h[l_ceR] -= i_scaling * l_netUpdates[1][0];
      m_hv[l_ceR] -= i_scaling * l_netUpdates[1][1];
    } 
  }
}

void tsunami_lab::patches::WavePropagation2d::setGhostRow()
{
  // bottom row & top row
  if (m_choiceBoundry)
  {
    for (t_idx l_g = 1; l_g < m_xCells + 1; l_g++)
    {
      m_h[l_g] = m_h[getIndex(l_g, 1)];
      m_h[getIndex(l_g, m_yCells + 1)] = m_h[getIndex(l_g, m_yCells)];
      m_hv[l_g] = -m_hv[getIndex(l_g, 1)];
      m_hv[getIndex(l_g, m_yCells + 1)] = -m_hv[getIndex(l_g, m_yCells)];
      m_b[l_g] = m_b[getIndex(l_g, 1)];
      m_b[getIndex(l_g, m_yCells + 1)] = m_b[getIndex(l_g, m_yCells)];
    }
  }
  else
  {
    for (t_idx l_g = 1; l_g < m_xCells + 1; l_g++)
    {
      m_h[l_g] = m_h[getIndex(l_g, 1)];
      m_h[getIndex(l_g, m_yCells + 1)] = m_h[getIndex(l_g, m_yCells)];
      m_hv[l_g] = -m_hv[getIndex(l_g, 1)];
      m_hv[getIndex(l_g, m_yCells + 1)] = -m_hv[getIndex(l_g, m_yCells)];
      m_b[l_g] = m_b[getIndex(l_g, 1)];
      m_b[getIndex(l_g, m_yCells + 1)] = m_b[getIndex(l_g, m_yCells)];
    }
  }
}

void tsunami_lab::patches::WavePropagation2d::setGhostCollumn()
{
  // leftest and rightest column
  if (m_choiceBoundry)
  {
    for (t_idx l_g = 1; l_g < m_yCells + 1; l_g++)
    {
      m_h[getIndex(0, l_g)] = m_h[getIndex(1, l_g)];
      m_h[getIndex(m_xCells + 1, l_g)] = m_h[getIndex(m_xCells, l_g)];
      m_hu[getIndex(0, l_g)] = -m_hu[getIndex(1, l_g)];
      m_hu[getIndex(m_xCells + 1, l_g)] = -m_hu[getIndex(m_xCells, l_g)];
      m_b[getIndex(0, l_g)] = m_b[getIndex(1, l_g)];
      m_b[getIndex(m_xCells + 1, l_g)] = m_b[getIndex(m_xCells, l_g)];
    }
  }
  else
  {
    for (t_idx l_g = 1; l_g < m_yCells + 1; l_g++)
    {
      m_h[getIndex(0, l_g)] = m_h[getIndex(1, l_g)];
      m_h[getIndex(m_xCells + 1, l_g)] = m_h[getIndex(m_xCells, l_g)];
      m_hu[getIndex(0, l_g)] = m_hu[getIndex(1, l_g)];
      m_hu[getIndex(m_xCells + 1, l_g)] = m_hu[getIndex(m_xCells, l_g)];
      m_b[getIndex(0, l_g)] = m_b[getIndex(1, l_g)];
      m_b[getIndex(m_xCells + 1, l_g)] = m_b[getIndex(m_xCells, l_g)];
    }
  }
}
