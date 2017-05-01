#ifndef _PANORAMAIMAGESTITCHING_H_
#define _PANORAMAIMAGESTITCHING_H_

#include <iostream>
#include <string>
#include <vector>

#include "CImg.h"

using std::string;
using std::vector;
using namespace cimg_library;

class PanoramaImageStitching {
  private:
    vector<CImg<double> > src;
    CImg<double> target;
  public:
    PanoramaImageStitching();
    bool loadOneImage(const string&);
    bool stitching();
    void showSrc();
    void showTarget();
    bool saveTarget(const string&);
};

#endif
