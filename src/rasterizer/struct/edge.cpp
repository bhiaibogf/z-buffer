//
// Created by bhiaibogf on 2022/1/5.
//

#include "edge.h"

Edge::Edge() {
    // TODO
}

void Edge::CopyToActiveEdge(float &x, float &dx, int &line) const {
    x = x_;
    dx = dx_;
    line = line_;
}
