//
// Created by bhiaibogf on 2022/1/5.
//

#include "edge.h"

Edge::Edge(int id, const Eigen::Vector4f &vertex_0, const Eigen::Vector4f &vertex_1) {
    id_ = id;

    if (vertex_0.y() < vertex_1.y()) {
        vertex_ = vertex_0;
        dx_ = (vertex_1.x() - vertex_0.x()) / (vertex_1.y() - vertex_0.y());
        // dx_ = (std::floor(vertex_1.x()) - std::floor(vertex_0.x())) /
        //       (std::floor(vertex_1.y()) - std::floor(vertex_0.y()));
        assert(abs(dx_) < 1e3 || (int(vertex_1.y()) - int(vertex_0.y()) == 0));
        Clip();
        line_ = int(vertex_1.y()) - int(vertex_.y());
    } else {
        vertex_ = vertex_1;
        dx_ = (vertex_0.x() - vertex_1.x()) / (vertex_0.y() - vertex_1.y());
        // dx_ = (std::floor(vertex_0.x()) - std::floor(vertex_1.x())) /
        //       (std::floor(vertex_0.y()) - std::floor(vertex_1.y()));
        assert(abs(dx_) < 1e3 || (int(vertex_1.y()) - int(vertex_0.y()) == 0));
        Clip();
        line_ = int(vertex_0.y()) - int(vertex_.y());
    }
}

void Edge::CopyToActiveEdge(float &x, float &dx, int &line) const {
    x = vertex_.x();
    dx = dx_;
    line = line_ - 1;
}

bool Edge::operator<(const Edge &edge) const {
    return (vertex_.x() < edge.vertex_.x()) || ((vertex_.x() == edge.vertex_.x()) && (dx_ < edge.dx_));
}

void Edge::Clip() {
    if (vertex_.y() > 0) {
        return;
    }
    float y_add = std::ceil(0.f - vertex_.y());
    vertex_.x() += dx_ * y_add;
    vertex_.y() += y_add;
}
