//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_CAMERA_H
#define Z_BUFFER_CAMERA_H

#include <eigen3/Eigen/Eigen>

#include "model/object.h"

class Camera {
public:
    Camera();

    Camera(const Eigen::Vector3f &position, float fov, float aspect, float near, float far);

    void set_view(const Eigen::Vector3f &position);

    void set_projection(float fov, float aspect, float near, float far);

    float near() const { return near_; }

    float far() const { return far_; }

    void Transform(Object &source_object) const;

private:
    constexpr const static float kPi = 3.1415927;
    float near_, far_;
    Eigen::Matrix4f view_;
    Eigen::Matrix4f projection_;

    static Eigen::Matrix4f LookAt(const Eigen::Vector3f &position,
                                  const Eigen::Vector3f &look_at = Eigen::Vector3f(0, 0, 0),
                                  const Eigen::Vector3f &up = Eigen::Vector3f(0, 1, 0));

    static float Radius(float alpha);

    static Eigen::Matrix4f Perspective(float fov, float aspect, float near, float far);

};


#endif //Z_BUFFER_CAMERA_H
