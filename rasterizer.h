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

    void Clear();

    void Draw(const Camera &camera, const Mesh &_mesh);

    void Show();

private:
    std::vector<float> depth_buffer_;

    int width_, height_;
    float near_, far_;

    int GetIdx(int x, int y) const;

    static bool IsInsideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    static auto ComputeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    //Screen space rasterization
    void RasterizeTriangle(const Triangle &triangle);

    float LinearizeDepth(float depth);

};


#endif //Z_BUFFER_RASTERIZER_H
