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

void Camera::set_view(const Eigen::Vector3f &position, const Eigen::Vector3f &center, const Eigen::Vector3f &up) {
    view_ = LookAt(position, center, up);
}

void Camera::set_projection(float fov, float aspect, float near, float far) {
    near_ = near;
    far_ = far;
    projection_ = Perspective(fov, aspect, near, far);
}

void Camera::Transform(Object &source_object) const {
    source_object.Transform(projection_ * view_ * source_object.model());
}

Eigen::Matrix4f Camera::LookAt(const Eigen::Vector3f &eye,
                               const Eigen::Vector3f &center, const Eigen::Vector3f &up) {
#ifndef LEFT_HAND
    Eigen::Vector3f f = (center - eye).normalized(),
            s = f.cross(up).normalized(),
            u = s.cross(f).normalized();

    Eigen::Matrix4f view;
    view << s.x(), s.y(), s.z(), -s.dot(eye),
            u.x(), u.y(), u.z(), -u.dot(eye),
            -f.x(), -f.y(), -f.z(), f.dot(eye),
            0.f, 0.f, 0.f, 1.f;
#else
    Eigen::Vector3f f = (center - eye).normalized(),
            s = up.cross(f).normalized(),
            u = f.cross(s).normalized();

    Eigen::Matrix4f view;
    view << s.x(), s.y(), s.z(), -s.dot(eye),
            u.x(), u.y(), u.z(), -u.dot(eye),
            f.x(), f.y(), f.z(), -f.dot(eye),
            0.f, 0.f, 0.f, 1.f;
#endif

    return view;
}

float Camera::Radius(float alpha) {
    return alpha * kPi / 180;
}

Eigen::Matrix4f Camera::Perspective(float fov, float aspect, float near, float far) {
    float cot = 1 / std::tan(Radius(fov / 2));
    Eigen::Matrix4f projection;
    projection << cot / aspect, 0, 0, 0,
            0, cot, 0, 0,
            0, 0, -(far + near) / (far - near), -2 * far * near / (far - near),
            0, 0, -1, 0;
    // std::cout << projection << std::endl;
    return projection;
}
