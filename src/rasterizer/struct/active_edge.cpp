//
// Created by bhiaibogf on 2022/1/5.
//

#include "active_edge.h"

ActiveEdge::ActiveEdge(const Edge &edge_1, const Edge &edge_2,
                       const Eigen::Vector3f &normal, const Eigen::Vector3f &color) {
    id_ = edge_1.id();
    if (edge_1 < edge_2) {
        UpdateEdge(edge_1);
        UpdateEdge(edge_2);
    } else {
        UpdateEdge(edge_2);
        UpdateEdge(edge_1);
    }
    dz_x_ = -normal.x() / normal.z();
    dz_y_ = -normal.y() / normal.z();

    color_ = color;
}

void ActiveEdge::Update() {
    line_l_--;
    line_r_--;
    x_l_ += dx_l_;
    x_r_ += dx_r_;
    z_l_ += dz_x_ * dx_l_ + dz_y_;
}

void ActiveEdge::UpdateEdge(const Edge &edge) {
    if (line_l_ < 0) {
        edge.CopyToActiveEdge(x_l_, dx_l_, line_l_);
        z_l_ = edge.z();
    } else {
        edge.CopyToActiveEdge(x_r_, dx_r_, line_r_);
    }
}
