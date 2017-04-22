#ifndef _Ex5_FACEMORPHING_H_
#define _Ex5_FACEMORPHING_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "CImg.h"
#undef Success
#include <eigen3/Eigen/Dense>

using std::vector;
using std::string;
using namespace cimg_library;
using namespace Eigen;

const double DIFF = 0.00000001;
const int TS = 28;


struct Dot {
    double x, y;
    int value;
    Dot(const double& _x, const double& _y, const int& _value) {
        x = _x;
        y = _y;
        value = _value;
    }
    Dot(const double& _x, const double& _y) {
        x = _x;
        y = _y;
        value = -2;
    }
    bool operator==(const Dot& rhs) {
        return (fabs(x-rhs.x) < DIFF) && (fabs(y - rhs.y) < DIFF) \
        && (value == rhs.value);
    }
};

struct vector2d {
  double x,y;
  vector2d(const double& _x, const double& _y):x(_x), y(_y) {}
  double CrossProduct(const vector2d& rhs) {
      return (x*rhs.y-y*rhs.x);
  }
};

struct Triangle {
    Dot A, B, C;
    Triangle(const Dot& a, const Dot& b, const Dot& c):A(a), B(b), C(c) {}
    double ComputeArea() const {
        vector2d AB(B.x-A.x, B.y-A.y);
        vector2d AC(C.x-A.x, C.y-A.y);
        return fabs(AB.CrossProduct(AC)/2.0);
    }
};

class Ex5_FaceMorphing {
  public:
    Ex5_FaceMorphing(const string& src_filepath, const string& target_filepath);
    void buildDelaunayEx(const vector<Dot>&, vector<Triangle>&);
    // vecDotSrc, vecTriSrc
    void displaySourceImage_GetDots();
    // vecDotTar
    void displayTargetImage_GetDots();
    // vecDotSrc, veTriSrc
    void Delaunay_src();
    // vecDotTar, vecTriTar
    void Delaunay_target();
    void showSrcTarTri();
    bool readin_txt();
    bool output_txt();
    Triangle middleTriangle(const Triangle&, const Triangle&, const double&);
    Matrix3d TriangleToTriangle(const double& u0, const double& v0, const double& u1, \
            const double& v1, const double& u2, const double& v2 ,\
            const double& x0, const double& y0, const double& x1, const double& y1, \
            const double& x2, const double& y2);
    Matrix3d wTriangleToTriangle(const Triangle&, const Triangle&);
    void Trans();
    Dot getTriangleCircle(const Triangle&);

  private:
    string filenameSrc;
    string filenameTar;
    CImg<double> src;
    CImg<double> target;
    int src_w;
    int src_h;
    int target_w;
    int target_h;
    vector<Dot> vecDotSrc;
    vector<Dot> vecDotTar;
    vector<Triangle> vecTriSrc;
    vector<Triangle> vecTriTar;
    vector<Triangle> vecTriM;
    Matrix3d A;
    const int DOT_COLOR[3]{0,255,0};
    const int LINE_COLOR[3]{255,255,255};
    const double RATE = 12;
    const double ERR = 3;
    const int DOT_RADIUS = 4;

    bool isThreePointOnOneLine(const Dot&, const Dot&, const Dot&);
    bool pointInCircle(const Dot&, const Dot&);
    double DotDisntance(const Dot&, const Dot&);
    void MTranstion(const double&);
    bool pointInTriangle(const Triangle&, const Dot&);
    bool readin(const string&, vector<Dot>&, vector<Triangle>&);
};

#endif
