//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_VANILLA_H
#define Z_BUFFER_VANILLA_H

#include <vector>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>

#include "../utils/camera.h"
#include "../object/triangle.h"
#include "../object/mesh.h"

#include "rasterizer.h"

class Vanilla : public Rasterizer {
public:
    Vanilla(int width, int height);

    ~Vanilla() = default;

    void Clear() override;

    void FragmentShader() override;

    void Show() override;

private:
    std::vector<float> depth_buffer_;

    int GetIdx(int x, int y) const;

    static bool IsInsideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    static auto ComputeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    //Screen space rasterization
    void RasterizeTriangle(const Triangle &triangle);

};


#endif //Z_BUFFER_VANILLA_H
