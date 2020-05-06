#include "roversight/roversight.h"
#include <yaml.hpp>
#include <cassert>
#include <fmt/format.h>
#include <iostream>
#include <ostream>

namespace roversight {

Camera::Camera(const std::string& intrinsics_path) {
    Yaml::Node root;
    Yaml::Parse(root, intrinsics_path.c_str());

    assert(root["model"].As<std::string>() == "pinhole_radial_tangential");

    Yaml::Node& pinhole = root["pinhole"];
    fx = pinhole["fx"].As<float>();
    fy = pinhole["fy"].As<float>();
    cx = pinhole["cx"].As<float>();
    cy = pinhole["cy"].As<float>();

    width = pinhole["width"].As<float>();
    height = pinhole["height"].As<float>();

    skew = pinhole["skew"].As<float>();

    Yaml::Node& radial_tangential = root["radial_tangential"];
    Yaml::Node& radial = radial_tangential["radial"];
    r1 = radial[0].As<float>(0.0f);
    r2 = radial[1].As<float>(0.0f);
    r3 = radial[2].As<float>(0.0f);

    t1 = radial_tangential["t1"].As<float>(0.0f);
    t2 = radial_tangential["t2"].As<float>(0.0f);
}

std::ostream& operator<<(std::ostream& os, const Camera& cam) {
    os << "Camera Intrinsics:\n";
    os << "  f: (" << cam.fx << ", " << cam.fy << ")\n";
    os << "  c: (" << cam.cx << ", " << cam.cy << ")\n";
    os << "  w: " << cam.width << " h: " << cam.height << "\n"; 
    os << "  skew: " << cam.skew << "\n";
    os << "  r: [" << cam.r1 << ", " << cam.r2 << ", " << cam.r3 << "]\n";
    os << "  t: [" << cam.t1 << ", " << cam.t2 << "]\n";
    return os;
}

} // namespace roversight
