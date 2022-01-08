//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_VANILLA_H
#define Z_BUFFER_VANILLA_H

#include "../object/triangle.h"

#include "rasterizer.h"

class Vanilla : public Rasterizer {
public:
    Vanilla(int width, int height);

    ~Vanilla() = default;

    void FragmentShader() override;

private:
    static bool IsInsideTriangle(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    static auto ComputeBarycentric2D(float x, float y, const std::array<Eigen::Vector4f, 3> &vertices);

    //Screen space rasterization
    void RasterizeTriangle(const Triangle &triangle);

};


#endif //Z_BUFFER_VANILLA_H
