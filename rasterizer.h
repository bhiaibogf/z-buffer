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
#include "model/mesh.h"

class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer() = default;

    auto &camera() { return camera_; }

    void Clear();

    void Draw();

    void Show();

private:
    std::vector<float> depth_buffer_;

    int width_, height_;
    Camera camera_;

    Mesh mesh_ = Mesh({
                              {2,   0,   -2},
                              {0,   2,   -2},
                              {-2,  0,   -2},
                              {3.5, -1,  -5},
                              {2.5, 1.5, -5},
                              {-1,  0.5, -5}
                      }, {
                              {0, 1, 2},
                              {3, 4, 5}
                      });

    int GetIdx(int x, int y) const;

    static bool IsInsideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    static auto ComputeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    //Screen space rasterization
    void RasterizeTriangle(const Triangle &triangle);

    float LinearizeDepth(float depth);

};


#endif //Z_BUFFER_RASTERIZER_H
