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

    Eigen::Vector3f normal() const { return normal_; }

    static void Reset() { id_cnt_ = 0; }

    bool NeedDraw(int height) const { return line_ > 0 && min_y() < height; }

    bool InLine() const { return line_ == 0; }

    void Update() { line_--; }

    bool NeedDeleted() const { return line_ < 0; }

private:
    static int id_cnt_;

    int id_;

    Eigen::Vector3f normal_;

    float min_y_;

    int line_;

    static Eigen::Vector3f TriangleToPlane(const Triangle &triangle);

};


#endif //Z_BUFFER_POLYGON_H
