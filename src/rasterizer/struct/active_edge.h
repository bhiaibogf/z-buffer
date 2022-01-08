//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_ACTIVE_EDGE_H
#define Z_BUFFER_ACTIVE_EDGE_H


#include "edge.h"

class ActiveEdge {
public:
    ActiveEdge(const Edge &edge_1, const Edge &edge_2, const Eigen::Vector3f &normal, const Eigen::Vector3f &color);

    int id() const { return id_; }

    int l() const { return int(x_l_); }

    int r() const { return int(x_r_); }

    float z() const { return z_l_; }

    float dz() const { return dz_x_; }

    auto color() const { return color_; };

    void Update();

    bool NeedUpdate() const { return line_l_ < 0 || line_r_ < 0; }

    bool NeedDeleted() const { return line_l_ < 0 && line_r_ < 0; }

    void UpdateEdge(const Edge &edge);

private:
    int id_;

    int line_l_ = -1, line_r_ = -1;

    float x_l_, dx_l_;
    float x_r_, dx_r_;

    float z_l_, dz_x_, dz_y_;

    Eigen::Vector3f color_;

};


#endif //Z_BUFFER_ACTIVE_EDGE_H
