#include "Configuration.h"

std::string tsunami_lab::io::Configuration::readFromConfigString(std::string i_configVariable){
    
    std::ifstream f("configs/config.json");
    json data = json::parse(f); 
    return data[i_configVariable];
}

tsunami_lab::t_real tsunami_lab::io::Configuration::readFromConfigReal(std::string i_configVariable){
    
    std::ifstream f("configs/config.json");
    json data = json::parse(f); 
    return data[i_configVariable];
}

tsunami_lab::t_idx tsunami_lab::io::Configuration::readFromConfigIndex(std::string i_configVariable){
    
    std::ifstream f("configs/config.json");
    json data = json::parse(f); 
    return data[i_configVariable];
}

bool tsunami_lab::io::Configuration::readFromConfigBoolean(std::string  i_configVariable){
    
    std::ifstream f("configs/config.json");
    json data = json::parse(f); 
    return data[i_configVariable];
}

std::vector<std::string> tsunami_lab::io::Configuration::checkMissingKeys(std::vector<std::string> i_keys) {
    std::vector<std::string> missingKeys;
    std::string filePath = "configs/config.json";
    // Read JSON data from the file
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return missingKeys;
    }

    json jsonData;
    try {
        fileStream >> jsonData;
    } catch (const json::parse_error& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        fileStream.close();
        return missingKeys;
    }

    fileStream.close();

    // Check for missing keys
    for (const auto& key : i_keys) {
        if (!jsonData.contains(key)) {
            missingKeys.push_back(key);
        }
    }

    return missingKeys;
}

void tsunami_lab::io::Configuration::readStationsFromJson(std::vector<tsunami_lab::Station> & stations) {
    std::string filename = "configs/stations.json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    json json_data;
    file >> json_data;
    file.close();

    stations.clear(); 

    for (const auto& station_data : json_data["stations"]) {
        tsunami_lab::Station station;
        station.i_name = station_data["i_name"];
        station.i_x = station_data["i_x"];
        station.i_y = station_data["i_y"];
        stations.push_back(station);
    }
}

tsunami_lab::t_real  tsunami_lab::io::Configuration::getFrequency(){
    std::string filename = "configs/stations.json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 0;
    }
    json json_data;
    file >> json_data;
    file.close();
    return json_data["frequency"];
}