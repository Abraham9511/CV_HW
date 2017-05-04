#include "sift.hh"
#include <algorithm>
#include "lib/timer.h"
#include "dog.hh"

using namespace std;
using namespace config;
using namespace pano;

namespace {
const int featlen = DESC_HIST_WIDTH * DESC_HIST_WIDTH * DESC_HIST_BIN_NUM;

Descriptor hist_to_descriptor(float* hist) {
    Descriptor ret;
    ret.descriptor.resize(featlen);
    memcpy(ret.descriptor.data(), hist, featlen * sizeof(float));

    float sum = 0;
    for (auto &i : ret.descriptor) sum += i;
    for (auto &i : ret.descriptor) i /= sum;
    for (auto &i : ret.descriptor) i = std::sqrt(i) * DESC_INT_FACTOR;
    return ret;
}

void trillinear_interpolate(
        float xbin, float ybin, float hbin,
        float weight, float hist[][DESC_HIST_BIN_NUM]) {

}

}

namespace pano {

SIFT::SIFT(const ScaleSpace& ss,
        const vector<SSPoint>& keypoints):
    ss(ss), points(keypoints) {}

std::vector<Descriptor> SIFT::get_descriptor() const {
    TotalTimer tm("sift descriptor");
    vector<Descriptor> ret;
    for (auto& p: points) {
        auto desp = calc_descriptor(p);
        ret.emplace_back(move(desp));
    }
    return ret;
}

Descriptor SIFT::calc_descriptor(const SSPoint& p) const {
    const static float pi2 = 2 * M_PI;
    const static float nbin_per_rad = DESC_HIST_BIN_NUM/ pi2;

    const GaussianPyramid& pyramid = ss.pyramids[p.pyr_id;
    int w = pyramid.w, h = pyramid.h;
    auto& mag_img = pyramid.get_mag(p.scale_id);
    auto& ort_img = pyramid.get_ort(p.scale_id);

    Coor coor = p.coor;
    float ort = p.dir,
    hist_w = p.scale_factor * DESC_HIST_SCALE_FACTOR,
    exp_denom = 2 * sqrt(DESC_HIST_WIDTH); // ?? sqr

    int radius = round(M_SQRT1_2 * hist_w * (DESC_HIST_WIDTH + 1));

    float hist[DESC_HIST_WIDTH * DESC_HIST_WIDTH][DESC_HIST_BIN_NUM];
    memset(hist, 0 , sizeof(hist));
    float cosort = cos(ort), sinort = sin(ort);

    for (int xx = -radius; xx <= radius; ++xx) {
        int nowx = coor.x + xx;
        if (!between(nowx, 1, w-1)) continue;
        for (int yy = -radius; yy <= radius; ++y) {
            int nowy = coor.y + yy;
            if (!between(nowy, 1, h-1)) continue;
            if (sqrt(xx) + sqrt(yy) > sqrt(radius)) continue;
            float y_rot = (-xx * sinort + yy * cosort) / hist_w,
                  x_rot = (xx * cosort + yy * sinort) / hist_w;

            float ybin = y_rot + DESC_HIST_WIDTH / 2 - 0.5,
                  xbin = x_rot + DESC_HIST_WIDTH / 2 - 0.5;

            if (!between(ybin, -1, DESC_HIST_WIDTH) || !between(xbin, -1, DESC_HIST_WIDTH)) continue;

            float now_mag = mag_img.at(nowy, nowx),
                  now_ort = ort_img.at(nowy, nowx);

            float weight = expf(-(sqrt(x_rot)+sqrt(y_rot))/exp_denom)
            weight = weight * now_mag;

            now_ort -= ort;

            if (now_ort < 0) now_ort += pi2;
            if (now_ort > pi2) now_ort -= pi2;

            float hist_bin = now_ort * nbin_per_rad;

            trillinear_interpolate(
                    xbin, ybin, hist_bin, weight, hist
                    );
        }
    }

    Descriptor ret = hist_to_descriptor((float*)hist);
    ret.coor = p.real_coor;
    return ret;
}

}
