#pragma once
#ifndef ROVERMATH_H_
#define ROVERMATH_H_

#include <Eigen/Dense>
#include <cstdint>

namespace roversight {

using Vector3b = Eigen::Matrix<uint8_t, 3,1>;
using Vector3i = Eigen::Matrix<int32_t, 3,1>;
using Vector3f = Eigen::Matrix<float,   3,1>;
using Vector3d = Eigen::Matrix<double,  3,1>;
using Matrix3f = Eigen::Matrix<float,   3,3>;
using Matrix3d = Eigen::Matrix<double,  3,3>;
using Quaterniond = Eigen::Quaternion<double>;

using PixBuf3f = std::vector<Vector3f, Eigen::aligned_allocator<Vector3f>>;

} // namespace roversight
#endif // ROVERMATH_H_
