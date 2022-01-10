//
// Created by bhiaibogf on 2021/12/31.
//

#include "rasterizer.h"

Rasterizer::Rasterizer(int width, int height) : width_(width), height_(height) {
    near_ = 0.1;
    far_ = 50;

    depth_map_.resize(width * height);
    fragment_buffer_.resize(width * height);
}

void Rasterizer::Clear() {
    std::fill(depth_buffer_.begin(), depth_buffer_.end(), 1.f);
    std::fill(depth_map_.begin(), depth_map_.end(), 1.f);
    std::fill(fragment_buffer_.begin(), fragment_buffer_.end(), Eigen::Vector3f({-1, -1, -1}));
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

void Rasterizer::Draw(const Camera &camera, const Mesh &mesh) {
    VertexShader(camera, mesh);
    FragmentShader();
}

void Rasterizer::Show(bool show_depth) {
    if (show_depth) {
        cv::imshow("z-buffer", GetDepthMap());
        // std::cout << fragment_buffer_[GetIdx(268, 511 - 253)] << std::endl;
    } else {
        cv::imshow("z-buffer", GetFragmentBuffer());
    }
}

void Rasterizer::Save(bool show_depth) {
    if (show_depth) {
        cv::imwrite("z-buffer(depth).exr", GetDepthMap());
    } else {
        cv::imwrite("z-buffer(normal).exr", GetFragmentBuffer());
    }
}

int Rasterizer::GetIdx(int x, int y) const {
    return (height_ - y - 1) * width_ + x;
}

float Rasterizer::LinearizeDepth(float depth) const {
    float z = depth * 2.f - 1.f;// Back to NDC
    return (2.f * near_ * far_) / (far_ + near_ - z * (far_ - near_));
}

cv::Mat Rasterizer::GetDepthMap() {
    for (auto &pixel: depth_map_) {
        pixel = (LinearizeDepth(pixel) - near_) / (far_ - near_);
    }
    cv::Mat image(height_, width_, CV_32FC1, depth_map_.data());
    return image;
}

cv::Mat Rasterizer::GetFragmentBuffer() {
    for (auto &pixel: fragment_buffer_) {
        pixel.x() = (pixel.x() + 1) / 2;
        pixel.y() = (pixel.y() + 1) / 2;
        pixel.z() = (pixel.z() + 1) / 2;
    }
    cv::Mat image(height_, width_, CV_32FC3, fragment_buffer_.data());
    // image.convertTo(image, CV_8UC3, 1.0f);
    return image;
}
