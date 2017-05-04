#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <map>
#include <cstring>
#include <fstream>

namespace config {

class ConfigParser {
  public:
    std::map<std::string, float> data;

    ConfigParser(const char* fname);

    float get(const std::string& s);
};

extern bool CYLINDER;


const int DESC_HIST_WIDTH = 4;
const int DESC_HIST_BIN_NUM = 8;


}
