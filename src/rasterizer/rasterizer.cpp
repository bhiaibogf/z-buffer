//
// Created by bhiaibogf on 2021/12/31.
//

#include "rasterizer.h"

Rasterizer::Rasterizer(int width, int height) : width_(width), height_(height) {
    near_ = 0.1;
    far_ = 50;
}

float Rasterizer::LinearizeDepth(float depth) const {
    float z = depth * 2.f - 1.f;// Back to NDC
    return (2.f * near_ * far_) / (far_ + near_ - z * (far_ - near_));
}
