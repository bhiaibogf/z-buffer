//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_POLYGON_H
#define Z_BUFFER_POLYGON_H


#include <vector>

#include "active_edge.h"

class Polygon {
public:
    void Update() {
        dy_--;
    }

    bool NeedDeleted() const {
        return dy_ < 0;
    }

    std::vector<ActiveEdge> Edge() {
        // TODO
    }

    float a_, b_, c_, d_;
    int id_;
    int dy_;

};


#endif //Z_BUFFER_POLYGON_H
