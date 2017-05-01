#include "PanoramaImageStitching.h"
#include <iostream>
#include "CImg.h"

using std::string;
using std::vector;
using namespace cimg_library;

PanoramaImageStitching::PanoramaImageStitching(){}

bool PanoramaImageStitching::loadOneImage(const string& filepath) {
    CImg<double> temp(filepath.c_str());
    src.push_back(temp);
    return true;
}

void PanoramaImageStitching::showSrc() {
}

void PanoramaImageStitching::showTarget() {
    CImgDisplay tarDisplay;
    tarDisplay.display(target);
}

bool PanoramaImageStitching::saveTarget(const string& targetName) {
    target.save_bmp(targetName.c_str());
    return true;
}
