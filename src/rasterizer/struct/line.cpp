//
// Created by bhiaibogf on 2022/1/10.
//

#include "line.h"

Line::Line(const Triangle &triangle, const Eigen::Vector3f &normal, const Eigen::Vector3f &color) {
    l_ = std::numeric_limits<float>::infinity();
    r_ = -std::numeric_limits<float>::infinity();
    for (auto &vertex: triangle.vertices()) {
        if (l_ > vertex.x()) {
            l_ = vertex.x();
            z_ = vertex.z();
        }
        r_ = std::max(r_, vertex.x());
    }
    dz_ = -normal.x() / normal.z();
    assert(abs(dz_) < 1e2);
    color_ = color;
}
