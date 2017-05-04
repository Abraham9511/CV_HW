#pragma once

#include "lib/config.hh"
#include "lib/mat.hh"





namespace pano {

struct BriefPattern;

struct Descriptor {
    Vec2D coor;
    std::vector<float> descriptor;

    float euclidean_sqr(const Descriptor& r, float now_thres) const {
        return pano::euclidean_sqr(descriptor.data(), r.descriptor.data(), (int)descriptor.size(), now_thres);
    }

    int hamming(const Descriptor& r) const {
        return pano::hamming(descriptor.data(), r.descriptor.data(), (int)descriptor.size());
    }
};

struct SSPoint {
    Coor coor;
    Vec2D real_coor;
    int pyr_id, scale_id;
    float dir;
    float scale_factor;
};

}
