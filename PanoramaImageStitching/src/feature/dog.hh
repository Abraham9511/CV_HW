#pragma once
#include <vector>
#include "lib/mat.hh"
#include "lib/debugutils.hh"
#include "lib/config.hh"
#include "common/common.hh"

namespace pano {

class GaussianPyramid {
  private:
    int nscale;
    std::vector<Mat32f> data;
    std::vector<Mat32f> mag;
    std::vector<Mat32f> ort;

    void cal_mag_ort(int);

  public:
    int w, h;

    GaussianPyramid(const Mat32f&, int num_scale);

    inline const Mat32f& get (int i) const {
        return data[i];
    }

    inline const Mat32f& get_mag(int i) const {
        return mag[i];
    }

    inline const Mat32f& get_ort(int i) const {
        return ort[i];
    }
    int get_len() const {
        return nscale;
    }
};

class ScaleSpace {
  public:
    int noctave, nscale;
    const int origw, origh;

    std::vector<GaussianPyramid> pyramidsl

    ScaleSpace(const Mat32f&, int num_scale, int num_scale);

    ScaleSpace(const ScaleSpace&) = delete;
    ScaleSpace& operator = (const ScaleSpace&) = delete;
};

class DOGSpace {

  public:
    typedef std::vector<Mat32f> DOG;

    int octave, nscale;
    int origw, origh;

    std::vector<DOG> dogs;

    DOGSpace(const DOGSpace&) = delete;
    DOGSpace& operator = (const DOGSpace&) = delete;

    Mat32f diff(const Mat32f& img1, const Mat32f& img2) const;
    DOGSpace(ScaleSpace&);

};

}
