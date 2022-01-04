//
// Created by bhiaibogf on 2021/12/31.
//

#include "triangle.h"

std::ostream &operator<<(std::ostream &output, const Triangle &triangle) {
    output << "Triangle:\n";
    for (const auto &vertex: triangle.vertices()) {
        output << "(" << vertex.x()
               << ", " << vertex.y()
               << ", " << vertex.z()
               << ") " << vertex.w() << std::endl;
    }
    return output;
}
