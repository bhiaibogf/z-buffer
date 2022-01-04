//
// Created by bhiaibogf on 2022/1/4.
//

#include "transformer.h"

Eigen::Matrix4f trans_former::GetScale(float scale_x, float scale_y, float scale_z) {
    Eigen::Matrix4f scale = Eigen::Matrix4f::Identity();
    scale << scale_x, 0, 0, 0,
            0, scale_y, 0, 0,
            0, 0, scale_z, 0,
            0, 0, 0, 1;
    return scale;
}

Eigen::Matrix4f trans_former::GetRotation(float angle) {
    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();

    angle = angle * 3.1415927f / 180.f;
    rotation << std::cos(angle), 0, std::sin(angle), 0,
            0, 1, 0, 0,
            -std::sin(angle), 0, std::cos(angle), 0,
            0, 0, 0, 1;

    return rotation;
}

Eigen::Matrix4f trans_former::GetTranslation(float translation_x, float translation_y, float translation_z) {
    Eigen::Matrix4f translation;
    translation << 1, 0, 0, translation_x,
            0, 1, 0, translation_y,
            0, 0, 1, translation_z,
            0, 0, 0, 1;
    return translation;
}
