//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_POLYGON_H
#define Z_BUFFER_POLYGON_H


#include <vector>
#include <limits>

#include "../../object/triangle.h"
#include "active_edge.h"

class Polygon {
public:
    explicit Polygon(const Triangle &triangle);

    ~Polygon() = default;

    int id() const { return id_; }

    int min_y() const { return min_y_; }

    void Update() { line_--; }

    bool NeedDeleted() const { return line_ < 0; }

    std::vector<Edge> Edges();

private:
    static int id_cnt_;

    int id_;

    float a_, b_, c_, d_;

    float min_y_;

    int line_;

    static Eigen::Vector4f TriangleToPlane(const Triangle &triangle);

};


#endif //Z_BUFFER_POLYGON_H
