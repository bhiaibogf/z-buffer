//
// Created by bhiaibogf on 2021/12/31.
//

#include "rasterizer.h"

Rasterizer::Rasterizer(int width, int height) : width_(width), height_(height) {
    near_ = 0.1;
    far_ = 50;
}

void Rasterizer::Draw(const Camera &camera, const Mesh &mesh) {
    VertexShader(camera, mesh);
    FragmentShader();
}

void Rasterizer::VertexShader(const Camera &camera, const Mesh &mesh) {
    near_ = camera.near();
    far_ = camera.far();

    mesh_ = mesh;
    // std::cout << mesh_ << std::endl;

    camera.Transform(mesh_);
    // std::cout << mesh_ << std::endl;

    // Homogeneous division
    for (auto &vertex: mesh_.vertices()) {
        vertex.x() /= vertex.w();
        vertex.y() /= vertex.w();
        vertex.z() /= vertex.w();
    }
    // std::cout << mesh_ << std::endl;

    // Viewport transformation
    for (auto &vertex: mesh_.vertices()) {
        vertex.x() = 0.5f * float(width_) * (vertex.x() + 1);
        vertex.y() = 0.5f * float(height_) * (vertex.y() + 1);
        // no need
        vertex.z() = 0.5f * (vertex.z() + 1);
    }
    // std::cout << mesh_ << std::endl;
}

float Rasterizer::LinearizeDepth(float depth) const {
    float z = depth * 2.f - 1.f;// Back to NDC
    return (2.f * near_ * far_) / (far_ + near_ - z * (far_ - near_));
}
