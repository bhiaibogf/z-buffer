//
// Created by bhiaibogf on 2021/12/31.
//

#include "mesh.h"

Mesh::Mesh(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles)
        : triangles_(triangles) {
    vertices_.resize(vertices.size());
    for (int i = 0; i < vertices.size(); i++) {
        vertices_[i] << vertices[i], 1.f;
    }
}

void Mesh::Transform(const Eigen::Matrix4f &mvp) {
    for (auto &vertex: vertices_) {
        vertex = mvp * vertex;
    }
}

std::vector<Triangle> Mesh::Triangles() const {
    std::vector<Triangle> triangles;
    for (auto triangle: triangles_) {
        triangles.emplace_back(vertices_[triangle.x()], vertices_[triangle.y()], vertices_[triangle.z()]);
    }
    return triangles;
}

std::ostream &operator<<(std::ostream &output, const Mesh &mesh) {
    output << "Mesh:\n";
    int idx = 0;
    for (const auto &triangle: mesh.Triangles()) {
        output << "Triangle " << idx++ << ":\n";
        for (const auto &vertex: triangle.vertices()) {
            output << "(" << vertex.x()
                   << ", " << vertex.y()
                   << ", " << vertex.z()
                   << ") " << vertex.w() << std::endl;
        }
    }
    return output;
}
