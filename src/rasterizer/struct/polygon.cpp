//
// Created by bhiaibogf on 2022/1/5.
//

#include "polygon.h"

int Polygon::id_cnt_ = 0;

Polygon::Polygon(const Triangle &triangle) {
    id_ = id_cnt_++;

    normal_ = TriangleToPlane(triangle);

    float max_y = -std::numeric_limits<float>::infinity(), min_y = std::numeric_limits<float>::infinity();
    for (auto &vertex: triangle.vertices()) {
        max_y = std::max(max_y, vertex.y());
        min_y = std::min(min_y, vertex.y());
    }
    line_ = int(max_y) - int(min_y);
    min_y_ = min_y;
}

Eigen::Vector3f Polygon::TriangleToPlane(const Triangle &triangle) {
    Eigen::Vector3f a = triangle.vertices()[1].head<3>() - triangle.vertices()[0].head<3>(),
            b = triangle.vertices()[2].head<3>() - triangle.vertices()[0].head<3>();
    return a.cross(b).normalized();
}
