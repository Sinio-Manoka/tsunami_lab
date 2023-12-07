#include "Station.h"

void tsunami_lab::io::Station::write(tsunami_lab::t_idx              i_x,
                                     tsunami_lab::t_idx              i_y,
                                     tsunami_lab::t_idx              i_time_in_seconds,
                                     tsunami_lab::t_real             i_water_height,
                                     tsunami_lab::t_real             i_water_hu,
                                     tsunami_lab::t_real             i_water_hv,
                                     std::string                     i_csv_path){

    std::ofstream io_stream(i_csv_path,std::ios::app);
    if (!io_stream.is_open()) {
        std::cerr << "Error opening file: " << i_csv_path << std::endl;
        return ; 
    }
    std::uintmax_t fileSize = std::filesystem::file_size(i_csv_path);
    if(fileSize == 0){
        io_stream << "x,y,water_height,water_hu,water_hv,time_in_seconds";
    }
    io_stream << "\n";
    io_stream << i_x << "," << i_y << "," << i_water_height << "," << i_water_hu << "," << i_water_hv << ","<< i_time_in_seconds;
    io_stream << std::flush;
}


std::string tsunami_lab::io::Station::Stringify() {
    
    std::string json_file_path = "configs/stations.json";
    // Open the JSON file
    std::ifstream json_file(json_file_path);

    if (!json_file.is_open()) {
        std::cerr << "Error opening JSON file: " << json_file_path << std::endl;
        return "";
    }

    // Read the contents of the JSON file into a string
    std::string json_string((std::istreambuf_iterator<char>(json_file)),
                            std::istreambuf_iterator<char>());

    // Close the JSON file
    json_file.close();

    return json_string;
}