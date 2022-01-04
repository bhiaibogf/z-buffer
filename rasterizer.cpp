//
// Created by bhiaibogf on 2021/12/31.
//

#include "rasterizer.h"

Rasterizer::Rasterizer(int width, int height) : width_(width), height_(height) {
    depth_buffer_.resize(width * height);
    near_ = 0.1;
    far_ = 50;
}

void Rasterizer::Clear() {
    std::fill(depth_buffer_.begin(), depth_buffer_.end(), std::numeric_limits<float>::infinity());
}

void Rasterizer::Draw(const Camera &camera, const Mesh &_mesh) {
    near_ = camera.near();
    far_ = camera.far();

    // std::cout << mesh_ << std::endl;
    Mesh mesh(_mesh);
    camera.Transform(mesh);
    // std::cout << mesh << std::endl;

    // Homogeneous division
    for (auto &vertex: mesh.vertices()) {
        vertex.x() /= vertex.w();
        vertex.y() /= vertex.w();
        vertex.z() /= vertex.w();
    }
    // std::cout << mesh << std::endl;

    // Viewport transformation
    for (auto &vertex: mesh.vertices()) {
        vertex.x() = 0.5f * float(width_) * (vertex.x() + 1);
        vertex.y() = 0.5f * float(height_) * (vertex.y() + 1);
        // no need
        // vertex.z() = 0.5f * (vertex.z() + 1);
    }
    // std::cout << mesh << std::endl;

    for (auto &triangle: mesh.Triangles()) {
        RasterizeTriangle(triangle);
    }
}

void Rasterizer::Show() {
    for (auto &pixel: depth_buffer_) {
        pixel = (LinearizeDepth(pixel) - near_) / (far_ - near_);
    }
    cv::Mat image(height_, width_, CV_32FC1, depth_buffer_.data());
    cv::imshow("z-buffer", image);
}

int Rasterizer::GetIdx(int x, int y) const {
    return (height_ - y - 1) * width_ + x;
}

bool Rasterizer::IsInsideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices) {
    Eigen::Vector3f v[3];
    for (int i = 0; i < 3; i++)
        v[i] = {vertices[i].x(), vertices[i].y(), 1.0};
    Eigen::Vector3f f0, f1, f2;
    f0 = v[1].cross(v[0]);
    f1 = v[2].cross(v[1]);
    f2 = v[0].cross(v[2]);
    Eigen::Vector3f p(x, y, 1.);
    if ((p.dot(f0) * f0.dot(v[2]) > 0) && (p.dot(f1) * f1.dot(v[0]) > 0) && (p.dot(f2) * f2.dot(v[1]) > 0))
        return true;
    return false;
}

auto Rasterizer::ComputeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices) {
    float c1 = (x * (vertices[1].y() - vertices[2].y()) + (vertices[2].x() - vertices[1].x()) * y +
                vertices[1].x() * vertices[2].y() - vertices[2].x() * vertices[1].y()) /
               (vertices[0].x() * (vertices[1].y() - vertices[2].y()) +
                (vertices[2].x() - vertices[1].x()) * vertices[0].y() + vertices[1].x() * vertices[2].y() -
                vertices[2].x() * vertices[1].y());
    float c2 = (x * (vertices[2].y() - vertices[0].y()) + (vertices[0].x() - vertices[2].x()) * y +
                vertices[2].x() * vertices[0].y() - vertices[0].x() * vertices[2].y()) /
               (vertices[1].x() * (vertices[2].y() - vertices[0].y()) +
                (vertices[0].x() - vertices[2].x()) * vertices[1].y() + vertices[2].x() * vertices[0].y() -
                vertices[0].x() * vertices[2].y());
    float c3 = (x * (vertices[0].y() - vertices[1].y()) + (vertices[1].x() - vertices[0].x()) * y +
                vertices[0].x() * vertices[1].y() - vertices[1].x() * vertices[0].y()) /
               (vertices[2].x() * (vertices[0].y() - vertices[1].y()) +
                (vertices[1].x() - vertices[0].x()) * vertices[2].y() + vertices[0].x() * vertices[1].y() -
                vertices[1].x() * vertices[0].y());
    return std::tuple(c1, c2, c3);
}

void Rasterizer::RasterizeTriangle(const Triangle &triangle) {
    auto &vertices = triangle.vertices();

    // find bounding box
    float min_x, min_y, max_x, max_y;
    min_x = float(width_);
    min_y = float(height_);
    max_x = max_y = 0;

    for (const auto &vertex: vertices) {
        min_x = std::min(min_x, vertex.x());
        min_y = std::min(min_y, vertex.y());
        max_x = std::max(max_x, vertex.x());
        max_y = std::max(max_y, vertex.y());
    }

    // rasterization loop
    int min_x_ = std::max(0, int(floor(min_x))), min_y_ = std::max(0, int(floor(min_y)));
    int max_x_ = std::min(width_ - 1, int(ceil(max_x))), max_y_ = std::min(height_ - 1, int(ceil(max_y)));

    for (int x = min_x_; x <= max_x_; x++) {
        for (int y = min_y_; y <= max_y_; y++) {
            float xx = 0.5f + float(x), yy = 0.5f + float(y);
            if (IsInsideTriangle(xx, yy, triangle.vertices())) {
                // 插值
                auto[alpha, beta, gamma] = ComputeBarycentric2D(xx, yy, triangle.vertices());

                float w_interpolated_reciprocal =
                        alpha / vertices[0].w() + beta / vertices[1].w() + gamma / vertices[2].w();
                float z_interpolated =
                        alpha * vertices[0].z() / vertices[0].w() + beta * vertices[1].z() / vertices[1].w() +
                        gamma * vertices[2].z() / vertices[2].w();
                z_interpolated /= w_interpolated_reciprocal;

                if (z_interpolated < depth_buffer_[GetIdx(x, y)]) {
                    depth_buffer_[GetIdx(x, y)] = z_interpolated;
                }
            }
        }
    }
}

float Rasterizer::LinearizeDepth(float depth) {
    float z = depth * 2.f - 1.f;// Back to NDC
    return (2.f * near_ * far_) / (far_ + near_ - z * (far_ - near_));
}
