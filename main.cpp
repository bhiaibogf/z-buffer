#include <iostream>

#include <eigen3/Eigen/Eigen>

#include "rasterizer.h"
#include "model/transformer.h"
#include "obj_loader.h"

int main(int argc, const char **argv) {
    Rasterizer rasterizer(512, 512);

    Eigen::Vector3f eye_pos = {0, 0, 10};
    Camera camera;

    Mesh mesh = obj_loader::LoadObj("objs/spot/spot_triangulated_good.obj");
    // Mesh mesh = Mesh({{2,   0,   -2},
    //                   {0,   2,   -2},
    //                   {-2,  0,   -2},
    //                   {3.5, -1,  -5},
    //                   {2.5, 1.5, -5},
    //                   {-1,  0.5, -5}}, {{0, 1, 2},
    //                                     {3, 4, 5}});
    // std::cout << mesh;

    int key = 0;
    int frame_count = 0;
    while (key != 27) {
        rasterizer.Clear();

        mesh.set_model(trans_former::GetTranslation() * trans_former::GetRotation() * trans_former::GetScale());

        camera.set_view(eye_pos);
        camera.set_projection(45, 1, 0.1, 50);

        rasterizer.Draw(camera, mesh);
        rasterizer.Show();
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}
