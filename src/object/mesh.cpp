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

    for (auto &triangle: triangles_) {
        colors_.push_back(Triangle::GetNormal(vertices[triangle.x()], vertices[triangle.y()], vertices[triangle.z()]));
    }
}

void Mesh::AddVertex(const Eigen::Vector3f &vertex) {
    vertices_.emplace_back(vertex.x(), vertex.y(), vertex.z(), 1.0);
}

void Mesh::AddFace(const Eigen::Vector3i &triangle) {
    triangles_.emplace_back(triangle.x() - 1, triangle.y() - 1, triangle.z() - 1);
}

void Mesh::GetColor() {
    for (auto triangle: triangles_) {
        colors_.push_back(Triangle::GetNormal(vertices_[triangle.x()].head<3>(),
                                              vertices_[triangle.y()].head<3>(),
                                              vertices_[triangle.z()].head<3>()));
    }
}

void Mesh::Transform(const Eigen::Matrix4f &mvp) {
    for (auto &vertex: vertices_) {
        vertex = mvp * vertex;
    }
}

std::vector<Triangle> Mesh::Triangles() const {
    std::vector<Triangle> triangles;
    int cnt = 0;
    for (auto triangle: triangles_) {
        triangles.emplace_back(vertices_[triangle.x()], vertices_[triangle.y()], vertices_[triangle.z()], colors_[cnt]);
        cnt++;
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

void Mesh::Count() const {
    std::cout << "Mesh has " << vertices_.size() << " points, " << triangles_.size() << " faces." << std::endl;
}
