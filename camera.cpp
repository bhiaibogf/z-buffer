//
// Created by bhiaibogf on 2021/12/31.
//

#include "camera.h"

Camera::Camera() {
    view_ = Eigen::Matrix4f::Identity();
    projection_ = Eigen::Matrix4f::Identity();
}

Camera::Camera(const Eigen::Vector3f &position, float fov, float aspect, float near, float far)
        : near_(near), far_(far) {
    set_view(position);
    set_projection(fov, aspect, near, far);
}

void Camera::set_view(const Eigen::Vector3f &position) {
    view_ = LookAt(position);
}

void Camera::set_projection(float fov, float aspect, float near, float far) {
    near_ = near;
    far_ = far;
    projection_ = Perspective(fov, aspect, near, far);
}

void Camera::Transform(Object &source_object) const {
    source_object.Transform(projection_ * view_ * source_object.model());
}

Eigen::Matrix4f Camera::LookAt(const Eigen::Vector3f &position,
                               const Eigen::Vector3f &look_at, const Eigen::Vector3f &up) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -position[0],
            0, 1, 0, -position[1],
            0, 0, 1, -position[2],
            0, 0, 0, 1;

    view = translate * view;

    return view;
}

float Camera::Radius(float alpha) {
    return alpha * kPi / 180;
}

Eigen::Matrix4f Camera::Perspective(float fov, float aspect, float near, float far) {
    float t = near * std::tan(Radius(fov / 2));
    float r = t * aspect;
    Eigen::Matrix4f projection;
    projection << near / r, 0, 0, 0,
            0, near / t, 0, 0,
            0, 0, -(far + near) / (far - near), -2 * far * near / (far - near),
            0, 0, -1, 0;
    // std::cout << projection << std::endl;
    return projection;
}
