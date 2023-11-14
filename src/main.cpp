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
#include "io/Csv/Csv.h"
#include "io/JsReader/Configuration.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

int main() {
  
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



  /*
  OLD:
  if( i_argc < 2 ) {

    std::cerr << "invalid number of arguments, usage:" << std::endl;
    std::cerr << "  ./build/tsunami_lab N_CELLS_X" << std::endl;
    std::cerr << "where N_CELLS_X is the number of cells in x-direction." << std::endl;
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
  */
  
  //NEW::
  //Errors checking-----------------------------------------------------------------------START
  //1. Does the Json File exists???
  const std::string filename = "configs/config.js";
  std::ifstream fileStream(filename.c_str());
  if (fileStream.good()) {
    std::cout << "\033[1;32m\u2713 The File 'config.js' does exists "  << std::endl;
  } else {
    std::cout << "\033[1;31m\u2717 The File 'config.js' does not exist under 'configs/config.js'  "<< std::endl;
    return EXIT_FAILURE;
  }
  //2. Are all the needed Keys there??
  std::vector<std::string> keysToCheck = {"solver", "dxy", "setup","nx"};
  std::vector<std::string> missingKeys = tsunami_lab::io::Configuration::checkMissingKeys(keysToCheck);
  if(missingKeys.size() > 0){
    std::cout << "\033[1;31m\u2717 Some Keys are missing. "  << std::endl;
    for (const auto& key : missingKeys) {
      std::cout << "-> "<< key << std::endl;
    }
    return EXIT_FAILURE;
  }else{
    std::cout << "\033[1;32m\u2713 All needed Keys do exists."  << std::endl;
    for (const auto& key : keysToCheck) {
      std::cout << "-> "<< key << std::endl;
    }
      std::cout << "\033[0m"; //reset the Terminal color From Green to White
  }
  //Errors checking-----------------------------------------------------------------------END
  //Declaration---------------------------------------------------------------------------START
  
  //New:: Reading the nx from the Json File
  l_nx =  tsunami_lab::io::Configuration::readFromConfigIndex("nx");
        
  //New:: Reading the dxy from the Json File
  tsunami_lab::t_real l_temp_dxy =  tsunami_lab::io::Configuration::readFromConfigReal("dxy");
  l_dxy = l_temp_dxy / l_nx;
  
  //New:: Reading the Setup from the Json File
  std::string l_temp_setup = tsunami_lab::io::Configuration::readFromConfigString("setup");
  tsunami_lab::setups::Setup *l_setup = new tsunami_lab::setups::TsunamiEvent1d(20); //default setup
  if(l_temp_setup == "tsunamievent1d"){
    std::cout << "\033[1;32m\u2713 Setup : TsunamiEvent1d \033[0m" << std::endl;
    l_setup = new tsunami_lab::setups::TsunamiEvent1d(20);  
  }
                                    
  // construct solver
  tsunami_lab::patches::WavePropagation *l_waveProp;
  //NEW:: Reading the Solver from the Json file 
  std::string l_solver = tsunami_lab::io::Configuration::readFromConfigString("solver");
  if(l_solver == "roe") {
    std::cout << "\033[1;32m\u2713 Solver :  Roe\033[0m" << std::endl;
    l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , true);
  }else{
    std::cout << "\033[1;32m\u2713 Solver : Fwave\033[0m" << std::endl;
    l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , false);
  }


  /*
   * if we get an arg then it checks what it is:
   *    if it is the word ROE written in any form (Roe, RoE, etc...) then it uses the Roe Solver
   *    else it uses the Fwave solver
   * if there is no arg given then it uses the Fwave solver. 
   */
  /*
    OLD:
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
  */

  //Declaration---------------------------------------------------------------------------END


  std::cout << "runtime configuration" << std::endl;
  std::cout << "  number of cells in x-direction: " << l_nx << std::endl;
  std::cout << "  number of cells in y-direction: " << l_ny << std::endl;
  std::cout << "  cell size:                      " << l_dxy << std::endl;



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

    //If true -> reflection boundary is active for the last cell
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
