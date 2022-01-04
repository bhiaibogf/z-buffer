//
// Created by bhiaibogf on 2021/12/31.
//

#include "triangle.h"

Triangle::Triangle(const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f &c) {
    vertices_[0] << a, 1.f;
    vertices_[1] << b, 1.f;
    vertices_[2] << c, 1.f;
}

Triangle::Triangle(const Eigen::Vector4f &a, const Eigen::Vector4f &b, const Eigen::Vector4f &c) {
    vertices_[0] << a;
    vertices_[1] << b;
    vertices_[2] << c;
}

void Triangle::Transform(const Eigen::Matrix4f &mvp) {
    for (auto &vertex: vertices_) {
        vertex = mvp * vertex;
    }
}

std::ostream &operator<<(std::ostream &output, const Triangle &triangle) {
    output << "Triangle:\n";
    for (const auto &vertex: triangle.vertices()) {
        output << "(" << vertex.x()
               << ", " << vertex.y()
               << ", " << vertex.z()
               << ") " << vertex.w() << std::endl;
    }
    return output;
}
