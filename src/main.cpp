/**
 * @author Alexander Breuer (alex.breuer AT uni-jena.de)
 *
 * @section DESCRIPTION
 * Entry-point for simulations.
 **/
#include "patches/WavePropagation1d.h"
#include "setups/dambreak/DamBreak1d.h"
#include "setups/rarerare/RareRare.h"
#include "setups/shockshock/ShockShock.h"
#include "setups/subcriticalflow/SubcriticalFlow.h"
#include "setups/supercriticalflow/SupercriticalFlow.h"
#include "setups/tsunamievent1d/TsunamiEvent1d.h"
#include "io/Csv.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <string>

int main( int   i_argc,
          char *i_argv[] ) {
  // number of cells in x- and y-direction
  tsunami_lab::t_idx l_nx = 0;
  tsunami_lab::t_idx l_ny = 1;

  // set cell size
  tsunami_lab::t_real l_dxy = 25;

  std::cout << "####################################" << std::endl;
  std::cout << "### Tsunami Lab                  ###" << std::endl;
  std::cout << "###                              ###" << std::endl;
  std::cout << "### https://scalable.uni-jena.de ###" << std::endl;
  std::cout << "####################################" << std::endl;

  if( i_argc < 3 ) {
    std::cerr << "invalid number of arguments, usage:" << std::endl;
    std::cerr << "  ./build/tsunami_lab N_CELLS_X" << std::endl;
    std::cerr << "where N_CELLS_X is the number of cells in x-direction." << std::endl;
    std::cerr << "Solver Choice is field so please make an Input by adding ROE or FWAVE !!" << std::endl;
    return EXIT_FAILURE;
  }
  else {
    l_nx = atoi( i_argv[1] );
    if( l_nx < 1 ) {
      std::cerr << "invalid number of cells" << std::endl;
      return EXIT_FAILURE;
    }
    l_dxy = 440500.0 / l_nx;
  }
  std::cout << "runtime configuration" << std::endl;
  std::cout << "  number of cells in x-direction: " << l_nx << std::endl;
  std::cout << "  number of cells in y-direction: " << l_ny << std::endl;
  std::cout << "  cell size:                      " << l_dxy << std::endl;


  tsunami_lab::setups::Setup *l_setup;
  l_setup = new tsunami_lab::setups::TsunamiEvent1d(20);
                                                

  // construct solver
  tsunami_lab::patches::WavePropagation *l_waveProp;

  /*
   * if we get an arg then it checks what it is:
   *    if it is the word ROE written in any form (Roe, RoE, etc...) then it uses the Roe Solver
   *    else it uses the Fwave solver
   * if there is no arg given then it uses the Fwave solver. 
   */
  if (i_argc > 2) {
      for (char* ptr = i_argv[2]; *ptr; ++ptr) {
          *ptr = std::tolower(static_cast<unsigned char>(*ptr));
      }
      if (std::strstr(i_argv[2], "roe") != nullptr) {
          std::cout << "ROE Will be Used" << std::endl;
          l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , true);
      } else {
          std::cout << "FWAVE Will be Used" << std::endl;
          l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , false);
      }
  }else{
    std::cout << "FWAVE Will be Used" << std::endl;
    l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , false);
  }  



  // maximum observed height in the setup
  tsunami_lab::t_real l_hMax = std::numeric_limits< tsunami_lab::t_real >::lowest();

  // set up solver
  for( tsunami_lab::t_idx l_cy = 0; l_cy < l_ny; l_cy++ ) {
    tsunami_lab::t_real l_y = l_cy * l_dxy; 

    for( tsunami_lab::t_idx l_cx = 0; l_cx < l_nx; l_cx++ ) {
      tsunami_lab::t_real l_x = l_cx * l_dxy ;

      


      // get initial values of the setup
      tsunami_lab::t_real l_h = l_setup->getHeight( l_x,
                                                    l_y );
      l_hMax = std::max( l_h, l_hMax );

      tsunami_lab::t_real l_hu = l_setup->getMomentumX( l_x,
                                                        l_y );
      tsunami_lab::t_real l_hv = l_setup->getMomentumY( l_x,
                                                        l_y );

      tsunami_lab::t_real l_bv = l_setup->getBathymetry(l_x,
                                                        l_y );                                       

      // set initial values in wave propagation solver
      l_waveProp->setHeight( l_cx,
                             l_cy,
                             l_h );

      l_waveProp->setMomentumX( l_cx,
                                l_cy,
                                l_hu );

      l_waveProp->setMomentumY( l_cx,
                                l_cy,
                                l_hv );

      l_waveProp->setBathymetry( l_cx,
                                 l_cy,
                                 l_bv);
          

    }
  }

  // derive maximum wave speed in setup; the momentum is ignored
  tsunami_lab::t_real l_speedMax = std::sqrt( 9.81 * l_hMax );

  // derive constant time step; changes at simulation time are ignored
  tsunami_lab::t_real l_dt = 0.5 * l_dxy / l_speedMax;

  // derive scaling for a time step
  tsunami_lab::t_real l_scaling = l_dt / l_dxy;

  // set up time and print control
  tsunami_lab::t_idx  l_timeStep = 0;
  tsunami_lab::t_idx  l_nOut = 0;
  tsunami_lab::t_real l_endTime = 3600;
  tsunami_lab::t_real l_simTime = 0;

  std::cout << "entering time loop" << std::endl;

  // iterate over time
  while( l_simTime < l_endTime ){
    if( l_timeStep % 25 == 0 ) {
      std::cout << "  simulation time / #time steps: "
                << l_simTime << " / " << l_timeStep << std::endl;

      std::string l_path = "output/solution_" + std::to_string(l_nOut) + ".csv";
      std::cout << "  writing wave field to " << l_path << std::endl;

      std::ofstream l_file;
      l_file.open( l_path  );

      tsunami_lab::io::Csv::write( l_dxy,
                                   l_nx,
                                   1,
                                   1,
                                   l_waveProp->getHeight(),
                                   l_waveProp->getMomentumX(),
                                   nullptr,
                                   l_waveProp->getBathymetry(),
                                   l_file );
      l_file.close();
      l_nOut++;
    }

    //If true -> reflection boundary is active
    l_waveProp->setGhostOutflow(false);
    l_waveProp->timeStep( l_scaling );

    l_timeStep++;
    l_simTime += l_dt;
  }

  std::cout << "finished time loop" << std::endl;

  // free memory
  std::cout << "freeing memory" << std::endl;
  delete l_setup;
  delete l_waveProp;

  std::cout << "finished, exiting" << std::endl;
  return EXIT_SUCCESS;
}
