#include "Station.h"

void tsunami_lab::io::Station::write(tsunami_lab::t_real         i_x,
                                tsunami_lab::t_real              i_y,
                                tsunami_lab::t_idx              i_time_in_seconds,
                                tsunami_lab::t_idx              i_stride,
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
        io_stream << ",time_in_seconds";
    }
    io_stream << "\n";
    t_idx l_id = i_y * i_stride + i_x; 
    io_stream << i_x << "," << i_y;
    if( i_water_height != nullptr ) io_stream << "," << i_water_height[l_id];
    io_stream << "," << i_time_in_seconds;

    io_stream << std::flush;

}