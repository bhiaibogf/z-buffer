#include <iostream>

#include <eigen3/Eigen/Eigen>

#include "rasterizer.h"

int main(int argc, const char **argv) {
    Rasterizer rasterizer(1280, 720);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    int key = 0;
    int frame_count = 0;
    while (key != 27) {
        rasterizer.Clear();

        // r.set_model(get_model_matrix(angle));
        // r.set_model(get_rotation(Vector3f(1, 1, 1), angle));
        rasterizer.camera().set_view(eye_pos);
        rasterizer.camera().set_projection(45, 1, 0.1, 50);

        rasterizer.Draw();
        rasterizer.Show();
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}
