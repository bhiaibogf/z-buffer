//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_MESH_H
#define Z_BUFFER_MESH_H

#include <vector>

#include "object.h"
#include "triangle.h"

class Mesh : public Object {
public:
    Mesh(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &triangles);

    ~Mesh() = default;

    auto &vertices() { return vertices_; }

    const auto &vertices() const { return vertices_; }

    void Transform(const Eigen::Matrix4f &mvp) override;

    std::vector<Triangle> Triangles() const;

    friend std::ostream &operator<<(std::ostream &output, const Mesh &mesh);

private:
    std::vector<Eigen::Vector4f> vertices_;
    std::vector<Eigen::Vector3i> triangles_;

};


#endif //Z_BUFFER_MESH_H
