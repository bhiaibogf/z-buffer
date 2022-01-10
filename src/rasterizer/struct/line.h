//
// Created by bhiaibogf on 2022/1/10.
//

#ifndef Z_BUFFER_LINE_H
#define Z_BUFFER_LINE_H


#include "../../object/triangle.h"

class Line {
public:
    Line(const Triangle &triangle, const Eigen::Vector3f &normal, const Eigen::Vector3f &color);

    ~Line() = default;

    int l() const { return int(l_); }

    int r() const { return int(r_); }

    float z() const { return z_; }

    float dz() const { return dz_; }

    auto &color() const { return color_; }

private:
    float l_, r_;
    float z_, dz_;

    Eigen::Vector3f color_;

};


#endif //Z_BUFFER_LINE_H
