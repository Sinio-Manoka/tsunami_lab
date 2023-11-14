#ifndef TSUNAMI_LAB_IO_CONFIGURATION
#define TSUNAMI_LAB_IO_CONFIGURATION

#include "../../constants.h"
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>

using json = nlohmann::json;

namespace tsunami_lab {
  namespace io {
    class Configuration;
  }
}

class tsunami_lab::io::Configuration {

    public:
        static std::string readFromConfigString(std::string i_configVariable);
        static tsunami_lab::t_real readFromConfigReal(std::string i_configVariable);
        static bool readFromConfigBoolean(std::string i_configVariable);
        static t_idx readFromConfigIndex(std::string i_configVariable);
        static std::vector<std::string> checkMissingKeys(std::vector<std::string> i_keys);
};
#endif