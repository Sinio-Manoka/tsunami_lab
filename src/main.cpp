#include "patches/wavepropagation1d/WavePropagation1d.h"
#include "setups/tsunamievent2d/TsunamiEvent2d.h"
#include "patches/wavepropagation2d/WavePropagation2d.h"
#include "setups/ArtificialTsunami2d/ArtificialTsunami2d.h"
#include "setups/dambreak/DamBreak1d.h"
#include "setups/dambreak2d/DamBreak2d.h"
#include "setups/rarerare/RareRare.h"
#include "setups/shockshock/ShockShock.h"
#include "setups/subcriticalflow/SubcriticalFlow.h"
#include "setups/supercriticalflow/SupercriticalFlow.h"
#include "setups/tsunamievent1d/TsunamiEvent1d.h"
#include "util/Timer.h"
#include "io/Csv/Csv.h"
#include "io/NetCdf/NetCdf.h"
#include "io/Stations/Station.h"
#include "io/JsReader/Configuration.h"
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <chrono>
#include <thread>




void updateProgressBar(double current, double total, const std::chrono::high_resolution_clock::time_point& start_time, int width = 50) {
    const double progress = std::min(std::max((current / total) * 100.0, 0.0), 100.0);
    const int numHashes = static_cast<int>((progress / 100.0) * width);

    std::cout << "\r\033[1;33mProgress: ["
              << std::string(numHashes, '#') << std::string(width - numHashes, ' ')
              << "] " << static_cast<int>(progress) << "%";

    if (current > 0.0) {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        const int remaining_time = static_cast<int>((100.0 - progress) / (progress / elapsed_time));

        const int remaining_hours = remaining_time / 3600;
        const int remaining_minutes = (remaining_time % 3600) / 60;
        const int remaining_seconds = remaining_time % 60;

        std::cout << " | Estimated time remaining: "
                  << remaining_hours << " hours, "
                  << remaining_minutes << " minutes, "
                  << remaining_seconds << " seconds" << "\033[0m";
    }

    std::cout.flush();
}


bool checkCheckpoint(const std::string& outputFolder, const std::string& checkpointsFolder, const std::string& outputFile) {
 
    std::string outputFullPath = outputFolder + "/" + outputFile;
    std::string checkpointFullPath = checkpointsFolder + "/" + outputFile;

    
    std::ifstream checkpointFile(checkpointFullPath);
    if (checkpointFile.is_open()) {
        return true;
    } else {
        std::cout << "Checkpoint file does not exist for " << outputFile << std::endl;
        return false;
    }
}

int main() {
  Timer timer; 
  tsunami_lab::t_idx l_nx = 0;
  tsunami_lab::t_idx l_ny = 1;
  tsunami_lab::t_real l_dxy = 25;

  
  std::cout << "\033[1;33m"<< "####################################" << std::endl;
  std::cout << "### Tsunami Lab                  ###" << std::endl;
  std::cout << "###                              ###" << std::endl;
  std::cout << "### https://scalable.uni-jena.de ###" << std::endl;
  std::cout << "####################################" << "\033[0m" << std::endl;
  
  //NEW::
  //Errors checking-----------------------------------------------------------------------START
  //1. Does the Json File exists???
  const std::string filename = "configs/config.json";
  std::ifstream fileStream(filename.c_str());
  if (fileStream.good()) {
    std::cout << "\n\033[1;32m\u2713 The file 'config.json' does indeed exist."  << std::endl;
  } else {
    std::cout << "\n\033[1;31m\u2717 The File 'config.json' does not exist under 'configs/config.json'  "<< std::endl;
    return EXIT_FAILURE;
  }
  //2. Are all the needed Keys there??
  std::vector<std::string> keysToCheck = {"solver","dimension_x","dimension_y", "setup",
                                          "nx","hu","location","hl","ny","domain_start_x",
                                          "domain_start_y","wavepropagation","endtime","writer","bathfile","disfile","outputfilename","usecheckpoint"};
  std::vector<std::string> missingKeys = tsunami_lab::io::Configuration::checkMissingKeys(keysToCheck);
  if(missingKeys.size() > 0){
    std::cout << "\033[1;31m\u2717 Some Keys are missing. "  << std::endl;
    for (const auto& key : missingKeys) {
      std::cout << "-> "<< key << std::endl;
    }
    return EXIT_FAILURE;
  }
  std::cout << "\033[0m"; 
  std::string folder_path = "outputs";
  std::string l_check_point_path= folder_path + "/cp";
  if (std::filesystem::exists(folder_path)){
    if(std::filesystem::exists(l_check_point_path)){
      bool isCpEmpty = std::filesystem::is_empty(l_check_point_path);
      if(isCpEmpty){
        std::filesystem::remove_all(folder_path);
        std::filesystem::create_directory(folder_path);
        std::filesystem::create_directory(l_check_point_path);
      }
    }else{
        std::filesystem::remove_all(folder_path);
        std::filesystem::create_directory(folder_path);
        std::filesystem::create_directory(l_check_point_path);
    }

  }else if(!std::filesystem::exists(folder_path)){
    std::filesystem::create_directory(folder_path);
    std::filesystem::create_directory(l_check_point_path);
  }

  if (std::filesystem::exists("stations")) std::filesystem::remove_all("stations");
  std::filesystem::create_directory("stations");
  //Errors checking-----------------------------------------------------------------------END
  //Declaration---------------------------------------------------------------------------START


  bool l_use_cp = tsunami_lab::io::Configuration::readFromConfigBoolean("usecheckpoint");
  std::string l_temp_outputfilename = tsunami_lab::io::Configuration::readFromConfigString("outputfilename");
  std::string l_temp_outputfile =  folder_path + "/" + l_temp_outputfilename;

  if(l_use_cp){
    if (!std::filesystem::exists(l_temp_outputfile)) {
      std::cout << "\033[1;31m\u2717 Cannot use Checkpoint " << "\033[0m"<< std::endl;
      std::cout << "\033[1;31mReason : there is no output file matchs the config file name" <<"\033[0m"<< std::endl;
      return EXIT_FAILURE; 
    }

  }


      
  //New:: Reading the length and Width from the Json File
  tsunami_lab::setups::Setup *l_setup = nullptr;
  
  //New:: Reading Data from the Json File
  l_nx =  tsunami_lab::io::Configuration::readFromConfigIndex("nx");
  l_ny =  tsunami_lab::io::Configuration::readFromConfigIndex("ny");
  // we are working on it asking breuer next time
  
  std::string l_temp_setup = tsunami_lab::io::Configuration::readFromConfigString("setup");
  std::string l_temp_solver = tsunami_lab::io::Configuration::readFromConfigString("solver");
  std::string l_temp_waveprop = tsunami_lab::io::Configuration::readFromConfigString("wavepropagation");
  tsunami_lab::t_real l_domain_start_x = tsunami_lab::io::Configuration::readFromConfigReal("domain_start_x");
  tsunami_lab::t_real l_domain_start_y = tsunami_lab::io::Configuration::readFromConfigReal("domain_start_y");
  tsunami_lab::t_real l_temp_hr=  tsunami_lab::io::Configuration::readFromConfigReal("hr");
  tsunami_lab::t_real l_temp_hl = tsunami_lab::io::Configuration::readFromConfigReal("hl");
  tsunami_lab::t_real l_temp_hu = tsunami_lab::io::Configuration::readFromConfigReal("hu");
  tsunami_lab::t_real l_temp_location = tsunami_lab::io::Configuration::readFromConfigReal("location");
  tsunami_lab::t_real l_temp_dimension_x =  tsunami_lab::io::Configuration::readFromConfigReal("dimension_x");
  tsunami_lab::t_real l_temp_dimension_y =  tsunami_lab::io::Configuration::readFromConfigReal("dimension_y");
  tsunami_lab::t_real l_frequency = tsunami_lab::io::Configuration::getFrequency();
  tsunami_lab::t_real l_temp_endtime = tsunami_lab::io::Configuration::readFromConfigReal("endtime");
  std::string l_temp_writer = tsunami_lab::io::Configuration::readFromConfigString("writer");
  std::string l_temp_bathFile = tsunami_lab::io::Configuration::readFromConfigString("bathfile");
  std::string l_temp_disFile = tsunami_lab::io::Configuration::readFromConfigString("disfile");

  const char * l_bathFile = l_temp_bathFile.c_str();
  const char * l_disFile = l_temp_disFile.c_str();
  const char * l_outputFile = l_temp_outputfile.c_str();
  
  std::vector<tsunami_lab::Station> l_stations;

  tsunami_lab::io::Configuration::readStationsFromJson(l_stations);
  l_dxy = l_temp_dimension_x / l_nx;
  

  //Declaration---------------------------------------------------------------------------END
  //Reading the Solver from the Json file-------------------------------------------------START
   
  bool l_solver;
  if(l_temp_solver == "roe") {
    std::cout << "\033[1;32m\u2713 Solver :  Roe\033[0m" << std::endl;
    l_solver = true;
  }else{
    std::cout << "\033[1;32m\u2713 Solver : Fwave\033[0m" << std::endl;
    l_solver = false;
  }
  //Reading the Solver from the Json file-------------------------------------------------END
  //Determine which setup and which wavepropagation to use--------------------------------START
  tsunami_lab::patches::WavePropagation *l_waveProp = nullptr;
  if(l_temp_waveprop == "2d"){
    l_waveProp = new tsunami_lab::patches::WavePropagation2d( l_nx, l_ny, l_solver);
    std::cout << "\033[1;32m\u2713 WavePropagation : 2d will be chosen \033[0m" << std::endl;
    if(l_temp_setup == "artificialtsunami2D")
    {
      std::cout << "\033[1;32m\u2713 Setup : ArtificialTsunami2D \033[0m" << std::endl;
      l_setup = new tsunami_lab::setups::ArtificialTsunami2d();
    }
    else if(l_temp_setup == "tsunamievent2d")
    {
      std::cout << "\033[1;32m\u2713 Setup : TsunamiEvent2d \033[0m" << std::endl;
      l_setup = new tsunami_lab::setups::TsunamiEvent2d(l_bathFile ,l_disFile);
    }
    else
    {
      std::cout << "\033[1;32m\u2713 Setup : dambreak2d \033[0m" << std::endl;
      l_setup = new tsunami_lab::setups::DamBreak2d();
    }
   
  }else if(l_temp_waveprop == "1d")
  {
      std::cout << "\033[1;32m\u2713 WavePropagation : 1d will be chosen \033[0m" << std::endl;
      l_waveProp = new tsunami_lab::patches::WavePropagation1d( l_nx , l_solver);
      if(l_temp_setup == "tsunamievent1d"){
        std::cout << "\033[1;32m\u2713 Setup : TsunamiEvent1d \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::TsunamiEvent1d();
      }else if(l_temp_setup == "dambreak1d"){
        std::cout << "\033[1;32m\u2713 Setup : dambreak1d \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::DamBreak1d(l_temp_hl ,l_temp_hr,l_temp_location); 
      }else if(l_temp_setup == "supercriticalflow"){
        std::cout << "\033[1;32m\u2713 Setup : SupercriticalFlow \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::SupercriticalFlow();
      }
      else if(l_temp_setup == "subcriticalflow"){
        std::cout << "\033[1;32m\u2713 Setup : SubcriticalFlow \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::SubcriticalFlow();
      }
      else if(l_temp_setup == "shockshock" || l_temp_setup =="rarerare" ){
        if(l_temp_setup == "shockshock" ){
          std::cout << "\033[1;32m\u2713 Setup : ShockShock \033[0m" << std::endl;
          l_setup = new tsunami_lab::setups::ShockShock(l_temp_hl ,l_temp_hu,l_temp_location);  
        }else{
          std::cout << "\033[1;32m\u2713 Setup : RareRare \033[0m" << std::endl;
          l_setup = new tsunami_lab::setups::RareRare(l_temp_hl ,l_temp_hu,l_temp_location);  
        }
      }else if(l_temp_setup == "dambreak1d"){
        
        std::cout << "\033[1;32m\u2713 Setup : dambreak1d \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::DamBreak1d(l_temp_hl ,l_temp_hr,l_temp_location); 

      }else if(l_temp_setup == "dambreak2d"){

        std::cout << "\033[1;32m\u2713 Setup : dambreak2d \033[0m" << std::endl;
        l_setup = new tsunami_lab::setups::DamBreak2d();
      }
  }
    //Determine which setup and which wavepropagation to use--------------------------------END
  std::cout << "\n\033[1;34m" << "runtime configuration" << std::endl;
  std::cout << "  number of cells in x-direction: " << l_nx << std::endl;
  std::cout << "  number of cells in y-direction: " << l_ny << std::endl;
  std::cout << "  cell width:                     " << l_dxy << "\033[0m"<< std::endl;

  // maximum observed height in the setup
  tsunami_lab::t_real l_hMax = std::numeric_limits< tsunami_lab::t_real >::lowest();
  // set up solver

  if(l_temp_waveprop == "1d")
  {
    l_ny = 1;
  }

  for( tsunami_lab::t_idx l_cy = 0; l_cy < l_ny; l_cy++ )
  { 
    tsunami_lab::t_real l_y = l_cy * l_dxy + l_domain_start_y;
  
    for( tsunami_lab::t_idx l_cx = 0; l_cx < l_nx; l_cx++ )
    {
      tsunami_lab::t_real l_x = l_cx * l_dxy + l_domain_start_x;

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
  
  tsunami_lab::t_real l_dt = 0.50 * l_dxy / l_speedMax;
  // derive scaling for a time step
  tsunami_lab::t_real l_scaling = l_dt/l_dxy;
  
  std::cout << "\033[1;34mTime step: " << l_dt << "\033[0m" << std::endl;

  tsunami_lab::t_real amount_time_steps = ceil(l_temp_endtime/l_dt);
  std::cout << "\033[1;34mAmount of Time steps: " << amount_time_steps << "\033[0m" << std::endl;

  // set up time and print control
  tsunami_lab::t_idx  l_timeStep = 0;
  tsunami_lab::t_real l_simTime = 0;
  tsunami_lab::t_real l_time_step_index = 0;
  tsunami_lab::t_real  l_current_frequency_time = l_frequency;
  std::cout << "\033[1;34mGentering time loop" << "\033[0m\n" << std::endl;
  

  // Checking if the "y" of each Station is set 0, else delete it from the vector.
  if(l_temp_waveprop == "1d" && l_stations.size() != 0){
    l_stations.erase(
    std::remove_if(l_stations.begin(), l_stations.end(), [&](const auto& station) {
        if (station.i_y != 0) {
            std::cout << "\033[1;31m\u2717 " << station.i_name << " has to have the Y set to 0 \033[0m " << std::endl;
            return true;
        }
        return false; 
    }),
    l_stations.end());
  }
  //stations removing out of boundary-------------------------------------------------------------start
  //removing out of boundary stations
  std::string l_stations_string;
  if(l_temp_waveprop == "2d"){
    l_stations.erase(
    std::remove_if(l_stations.begin(), l_stations.end(), [&](const auto& station) {
    if (station.i_x < l_domain_start_x || station.i_x >= l_temp_dimension_x + l_domain_start_x ||
        station.i_y < l_domain_start_y || station.i_y >= l_temp_dimension_y + l_domain_start_y) {
        std::cout << "\033[1;31m\u2717 " << station.i_name << " is out of boundary \033[0m " << std::endl;
        std::string l_foldername = "stations/"+station.i_name;
        l_stations_string= l_stations_string + l_foldername +"/"+ station.i_name+".csv"+"$$"; 
        return true; // Remove the station
    }
    std::cout << "\033[1;32m\u2713 " << station.i_name << " is in boundary \033[0m " << std::endl;
    return false; // Keep the station
    }),
    l_stations.end());
  
  }else{
    l_stations.erase(
    std::remove_if(l_stations.begin(), l_stations.end(), [&](const auto& station) {
    if (station.i_x < l_domain_start_x || station.i_x >= l_temp_dimension_x + l_domain_start_x) {
        std::cout << "\033[1;31m\u2717 " << station.i_name << " is out of boundary \033[0m " << std::endl;
        return true; // Remove the station
    }
    std::cout << "\033[1;32m\u2713 " << station.i_name << " is in boundary \033[0m " << std::endl;
    return false; // Keep the station
    }),
    l_stations.end());
  }

  //stations ---------------------------------------------------------------------------------end

  //create the netCdf file reader/writer
  tsunami_lab::io::NetCdf* l_netCdf = new tsunami_lab::io::NetCdf(l_nx,l_ny,l_outputFile);
  

  if(l_temp_writer == "netcdf"){
    l_netCdf->fillConstants(l_nx,
                            l_ny,
                            l_dxy,
                            l_domain_start_x,
                            l_domain_start_y,
                            l_waveProp->getStride(),
                            l_waveProp->getBathymetry(),
                            l_outputFile);
  }
  std::string l_checkPointName = "CheckPoint-" + l_temp_outputfilename;
  while( l_simTime < l_temp_endtime ){
    l_waveProp->setGhostOutflow(false);
    if( l_timeStep % 25 == 0 ) {
      if(l_temp_writer == "csv"){
        std::string l_path = "outputs/solution_" + std::to_string(l_time_step_index) + ".csv";
        std::ofstream l_file;
        l_file.open( l_path );
        tsunami_lab::io::Csv::write(l_dxy,
                                    l_nx,
                                    l_ny,
                                    l_waveProp->getStride(),
                                    l_domain_start_x,
                                    l_domain_start_y,
                                    l_temp_waveprop,
                                    l_waveProp->getHeight(),
                                    l_waveProp->getMomentumX(),
                                    l_waveProp->getMomentumY(),
                                    l_waveProp->getBathymetry(),
                                    l_file);

        l_file.close();
      }else{
        l_netCdf->updateFile( l_nx,
                              l_ny,
                              l_waveProp->getStride(),
                              l_time_step_index,
                              l_simTime,
                              l_waveProp->getHeight(),
                              l_waveProp->getMomentumX(),
                              l_waveProp->getMomentumY(),
                              l_outputFile);
                              
        l_netCdf->createCheckPoint(l_temp_solver,
                                  l_domain_start_x,
                                  l_domain_start_y,
                                  l_temp_dimension_x,
                                  l_temp_dimension_y,
                                  l_temp_endtime,
                                  l_simTime,
                                  l_frequency,
                                  l_dt,
                                  l_waveProp->getBathymetry(),
                                  l_waveProp->getHeight(),
                                  l_waveProp->getMomentumX(),
                                  l_waveProp->getMomentumY(),
                                  l_time_step_index,
                                  l_waveProp->getStride(),
                                  l_nx,
                                  l_ny,
                                  l_temp_setup,
                                  tsunami_lab::io::Station::Stringify(),
                                  l_checkPointName);
      }
    }

    //STATIONS_---------------------------------------------START 
    if(l_current_frequency_time <= l_simTime){
      for (const auto& station : l_stations) {
        std::string l_foldername = "stations/"+station.i_name;
        if (!std::filesystem::exists(l_foldername)){
          std::filesystem::create_directory(l_foldername);
        }
        //compute cell IDs
        tsunami_lab::t_idx l_ix = ((station.i_x - l_domain_start_x ) / l_dxy )+ l_waveProp->getGhostcellX();
        tsunami_lab::t_idx l_iy = ((station.i_y - l_domain_start_y ) / l_dxy )+ l_waveProp->getGhostcellY();
        if(l_temp_waveprop == "1d"){
          l_iy = 0; 
        }
        tsunami_lab::t_idx l_id = l_iy * l_waveProp->getStride() + l_ix;
        const tsunami_lab::t_real* l_water_height =  l_waveProp->getHeight();
        const tsunami_lab::t_real* l_water_hu =  l_waveProp->getMomentumX();
        const tsunami_lab::t_real* l_water_hv =  l_waveProp->getMomentumY();
        std::string l_station_path = l_foldername +"/"+ station.i_name+".csv"; 
        tsunami_lab::io::Station::write(l_ix,
                                        l_iy,
                                        l_simTime,
                                        l_water_height[l_id],
                                        l_water_hu[l_id],
                                        l_water_hv[l_id],
                                        l_station_path);
      }
      l_current_frequency_time = l_current_frequency_time + l_frequency;
    }
    //STATIONS----------------------------------------------END

    l_waveProp->timeStep( l_scaling);
    l_timeStep++;
    l_simTime += l_dt;
    updateProgressBar(l_simTime, l_temp_endtime,timer.getStartTime());

  }

  std::cout << "\n\n\033[1;32m\u2713 All solutions have been written to the Folder : 'outputs' " << std::endl;
  // free memory
  std::cout << "\033[1;32m\u2713 freeing memory" << std::endl;
  delete l_setup;
  delete l_waveProp;
  delete l_netCdf;
  std::cout << "\033[1;32m\u2713 finished, exiting \033[0m" << std::endl;
  return EXIT_SUCCESS;
}