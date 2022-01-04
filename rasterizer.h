//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_RASTERIZER_H
#define Z_BUFFER_RASTERIZER_H

#include <vector>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>

#include "camera.h"
#include "model/triangle.h"

class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer() = default;

    auto &camera() { return camera_; }

    void Clear();

    void Draw();

    void Show() {
        for (auto &pixel: depth_buffer_) {
            pixel /= camera_.far();
        }
        cv::Mat image(height_, width_, CV_32FC1, depth_buffer_.data());
        // image.convertTo(image, CV_8UC1);
        cv::imshow("z-buffer", image);
    }

private:
    std::vector<float> depth_buffer_;

    int width_, height_;
    Camera camera_;

    Triangle triangle_ = Triangle({{2,  0, -2},
                                   {0,  2, -1},
                                   {-2, 0, -2}});

    int GetIdx(int x, int y) const;

    static bool insideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &_v);

    static auto computeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &v);

    //Screen space rasterization
    void RasterizeTriangle(const Triangle &triangle);

};


#endif //Z_BUFFER_RASTERIZER_H
