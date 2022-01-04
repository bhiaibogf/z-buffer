//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_TRIANGLE_H
#define Z_BUFFER_TRIANGLE_H

#include "object.h"

class Triangle : public Object {
public:
    Triangle(const Eigen::Vector3f &a, const Eigen::Vector3f &b, const Eigen::Vector3f &c);

    Triangle(const Eigen::Vector4f &a, const Eigen::Vector4f &b, const Eigen::Vector4f &c);

    ~Triangle() = default;

    auto &vertices() { return vertices_; }

    const auto &vertices() const { return vertices_; }

    void Transform(const Eigen::Matrix4f &mvp) override;

    friend std::ostream &operator<<(std::ostream &output, const Triangle &triangle);

private:
    std::array<Eigen::Vector4f, 3> vertices_;

};


#endif //Z_BUFFER_TRIANGLE_H
