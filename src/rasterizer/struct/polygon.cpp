//
// Created by bhiaibogf on 2022/1/5.
//

#include "polygon.h"

int Polygon::id_cnt_ = 0;

Polygon::Polygon(const Triangle &triangle) {
    id_ = id_cnt_++;

    auto coff = TriangleToPlane(triangle);
    a_ = coff.x();
    b_ = coff.y();
    c_ = coff.z();
    d_ = coff.w();

    float max_y = -std::numeric_limits<float>::infinity(), min_y = std::numeric_limits<float>::infinity();
    for (auto &vertex: triangle.vertices()) {
        max_y = std::max(max_y, vertex.y());
        min_y = std::min(min_y, vertex.y());
    }
    line_ = int(max_y) - int(min_y);
    min_y_ = min_y;
}

Eigen::Vector4f Polygon::TriangleToPlane(const Triangle &triangle) {
    return {}; // TODO
}

std::vector<Edge> Polygon::Edges() {
    return std::vector<Edge>(); // TODO
}
