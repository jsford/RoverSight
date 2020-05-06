#pragma once
#ifndef ROVERSIGHT_H_
#define ROVERSIGHT_H_

#include "rovermath.h"
#include <cstdint>
#include <vector>

namespace roversight {

struct Camera {
    float fx, fy;  
    float cx, cy;
    int width, height;       
    float skew;    
    float r1, r2, r3;
    float t1, t2;

    explicit Camera(const std::string& intrinsics_path);
};
std::ostream& operator<<(std::ostream& os, const Camera& cam);

struct Pose {
    Vector3d pos;
    Quaterniond ori;
};

class ImageRGB {
    public:
    int rows = 0;
    int cols = 0;

    Vector3f& at(const int ir, const int ic);
    Vector3f at(const int ir, const int ic) const;
    Vector3f at(const float fr, const float fc) const;

    ImageRGB() = default;
    ImageRGB(const int rows, const int cols);

    void from_file(const std::string& filename);
    void to_file(const std::string& filename) const;

    ImageRGB undistort(const Camera& cam) const;

    private:
    std::vector<Vector3f, Eigen::aligned_allocator<Vector3f>> pixels;
};

class Mesh {
};

class RoverSight {

    public:

    void addImage(const ImageRGB& img, const Camera& cam, const Pose& pose);
    //void addStereoImage(const StereoImage& sImg, const StereoCamera& sCam, const Pose& pose);

    void updateMesh();

    Mesh getMesh();

    private:
    std::vector<ImageRGB> images;
};

}   // namespace roversight
#endif  // ROVERSIGHT_H_
