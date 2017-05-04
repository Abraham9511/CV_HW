#ifndef _PANORAMAIMAGESTITCH_H_
#define _PANORAMAIMAGESTITCH_H_

#include <iostream>
#include <string>
#include <vector>

#include "CImg.h"

using std::string;
using std::vector;
using namespace cimg_library;

class PanoramaImageStitch {
  private:
    vector<CImg<double> > src;
    CImg<double> target;

  public:
    PanoramaImageStitch();
    bool loadOneImage(const string&);
    bool stitching();
    void showSrc();
    void showTarget();
    bool saveTarget(const string&);
};

#endif
