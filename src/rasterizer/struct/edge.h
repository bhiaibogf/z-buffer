//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_EDGE_H
#define Z_BUFFER_EDGE_H


#include <eigen3/Eigen/Core>

class Edge {
public:
    Edge() = default;

    Edge(int id, const Eigen::Vector4f &vertex_0, const Eigen::Vector4f &vertex_1);

    ~Edge() = default;

    bool operator<(const Edge &edge) const;

    int id() const { return id_; }

    int min_y() const { return int(std::max(vertex_.y(), 0.f)); }

    float z() const { return vertex_.z(); }

    bool NeedDraw(int height) const { return line_ > 0 && min_y() < height; }

    void CopyToActiveEdge(float &x, float &dx, int &line) const;

private:
    int id_;

    Eigen::Vector4f vertex_;

    float dx_;

    int line_;

    void Clip();

};


#endif //Z_BUFFER_EDGE_H
