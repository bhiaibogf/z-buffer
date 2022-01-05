//
// Created by bhiaibogf on 2022/1/4.
//

#ifndef Z_BUFFER_TRANSFORMER_H
#define Z_BUFFER_TRANSFORMER_H

#include <eigen3/Eigen/Eigen>

namespace trans_former {
    Eigen::Matrix4f GetScale(float scale_x = 0.f, float scale_y = 0.f, float scale_z = 0.f);

    Eigen::Matrix4f GetRotation(float angle = 0.f);

    Eigen::Matrix4f GetTranslation(float translation_x = 0, float translation_y = 0, float translation_z = 0);
}


#endif //Z_BUFFER_TRANSFORMER_H
