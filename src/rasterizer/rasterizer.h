//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_RASTERIZER_H
#define Z_BUFFER_RASTERIZER_H

#include <vector>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>

#include "../utils/camera.h"
#include "../object/mesh.h"

class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer() = default;

    virtual void Clear();

    void VertexShader(const Camera &camera, const Mesh &mesh);

    virtual void FragmentShader() = 0;

    void Draw(const Camera &camera, const Mesh &mesh);

    void Show(bool show_depth);

    void Save(bool show_depth);

protected:
    int width_, height_;
    float near_, far_;

    Mesh mesh_;

    std::vector<float> z_buffer_;
    std::vector<float> depth_buffer_;
    std::vector<Eigen::Vector3f> fragment_buffer_;

    cv::Mat *depth_mat_;
    cv::Mat *fragment_mat_;

    int GetIdx(int x, int y) const;

    float LinearizeDepth(float depth) const;

    cv::Mat GetDepthMap();

    cv::Mat GetFragmentBuffer();

};


#endif //Z_BUFFER_RASTERIZER_H
