//
// Created by bhiaibogf on 2022/1/5.
//

#include "polygon.h"

int Polygon::id_cnt_ = 0;

Polygon::Polygon(const Triangle &triangle) {
    id_ = id_cnt_++;

    normal_ = triangle.GetNormal();

    float max_y = -std::numeric_limits<float>::infinity(), min_y = std::numeric_limits<float>::infinity();
    for (auto &vertex: triangle.vertices()) {
        max_y = std::max(max_y, vertex.y());
        min_y = std::min(min_y, vertex.y());
    }
    min_y = std::max(min_y, 0.f);

    line_ = int(max_y) - int(min_y);
    min_y_ = min_y;

    color_ = triangle.color();
}
