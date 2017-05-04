#include "PanoramaImageStitch.h"
#include <iostream>
#include "CImg.h"

using std::string;
using std::vector;
using namespace cimg_library;

PanoramaImageStitch::PanoramaImageStitch(){}

bool PanoramaImageStitch::loadOneImage(const string& filepath) {
    CImg<double> temp(filepath.c_str());
    src.push_back(temp);
    return true;
}

void PanoramaImageStitch::showSrc() {
}

void PanoramaImageStitch::showTarget() {
    CImgDisplay tarDisplay;
    tarDisplay.display(target);
}

bool PanoramaImageStitch::saveTarget(const string& targetName) {
    target.save_bmp(targetName.c_str());
    return true;
}
