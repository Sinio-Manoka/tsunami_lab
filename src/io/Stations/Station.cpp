#include "Station.h"

void tsunami_lab::io::Station::write(tsunami_lab::t_real         i_x,
                                tsunami_lab::t_real             i_dxy,
                                tsunami_lab::t_real              i_y,
                                tsunami_lab::t_idx              i_nx,
                                tsunami_lab::t_idx              i_stride,
                                tsunami_lab::t_idx              i_ny,
                                tsunami_lab::t_real const *     i_water_height,
                                std::string                     i_csv_path){


    std::ofstream io_stream(i_csv_path,std::ios::app);
    if (!io_stream.is_open()) {
        std::cerr << "Error opening file: " << i_csv_path << std::endl;
        return ; 
    }
    std::uintmax_t fileSize = std::filesystem::file_size(i_csv_path);
    if(fileSize == 0){
        io_stream << "x,y";
        if( i_water_height != nullptr ) io_stream << ",water_height";
    }
    tsunami_lab::t_real domain_start = tsunami_lab::io::Configuration::readFromConfigReal("domain_start");
    io_stream << "\n";
    for( t_idx l_iy = 1; l_iy < i_ny+1; l_iy++ ) {
        for( t_idx l_ix = 1; l_ix < i_nx+1; l_ix++ ) {//l_ix=0 -> l_ix=1
        // derive coordinates of cell center
            t_real l_posX = ((l_ix-1 + 0.5) * i_dxy )+ domain_start; //l_ix -> l_ix-1
            t_real l_posY = ((l_iy-1 + 0.5) * i_dxy )+ domain_start;

            t_idx l_id = l_iy * i_stride + l_ix;
            //std::cout << "i_x" << i_x << ",i_y" << i_y << ",l_posX" << l_posX << ",l_posY" << l_posY<<std::endl;
            if(l_posX == i_x && l_posY == i_y){
                
                io_stream << l_posX << "," << l_posY;
                if( i_water_height != nullptr ) io_stream << "," << i_water_height[l_id];
            }

        }
    }
    io_stream << std::flush;

}