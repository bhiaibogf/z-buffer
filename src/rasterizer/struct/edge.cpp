//
// Created by bhiaibogf on 2022/1/5.
//

#include "edge.h"

Edge::Edge(int id, const Eigen::Vector4f &vertex_0, const Eigen::Vector4f &vertex_1) {
    id_ = id;

    if (vertex_0.y() < vertex_1.y()) {
        vertex_ = vertex_0;
        dx_ = (vertex_1.x() - vertex_0.x()) / (vertex_1.y() - vertex_0.y());
        line_ = int(vertex_1.y()) - int(vertex_0.y());
    } else {
        vertex_ = vertex_1;
        dx_ = (vertex_0.x() - vertex_1.x()) / (vertex_0.y() - vertex_1.y());
        line_ = int(vertex_0.y()) - int(vertex_1.y());
    }
}

void Edge::CopyToActiveEdge(float &x, float &dx, int &line) const {
    x = vertex_.x();
    dx = dx_;
    line = line_-1;
}

bool Edge::operator<(const Edge &edge) const {
    return vertex_.x() + dx_ < edge.vertex_.x() + edge.dx_;
}
