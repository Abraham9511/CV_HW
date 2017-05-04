#pragma once

#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include "utils.hh"
#include "debugutils.hh"


class Geometry {
  public:
    int w, h;
};

template<typename T>
class Vector {

};

template<typename T>
class Vector2D {

};

template<typename T>
std::ostream& operator << (std::ostream& os, const Vector2D<T>& v) {
    os << v.x << ' ' << v.y;
    return os;
}

typedef Vector<double> Vec;
typedef Vector2D<int> Coor;
typedef Vector2D<double> Vec2D;
