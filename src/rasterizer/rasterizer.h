//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_RASTERIZER_H
#define Z_BUFFER_RASTERIZER_H


#include "../utils/camera.h"
#include "../object/mesh.h"

class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer() = default;

    virtual void Clear() = 0;

    virtual void Draw(const Camera &camera, const Mesh &_mesh) = 0;

    virtual void Show() = 0;

protected:
    int width_, height_;
    float near_, far_;

    float LinearizeDepth(float depth) const;

};


#endif //Z_BUFFER_RASTERIZER_H
