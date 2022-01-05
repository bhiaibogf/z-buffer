#include <iostream>

#include <eigen3/Eigen/Eigen>

#include "utils/transformer.h"
#include "utils/obj_loader.h"
#include "object/mesh.h"
#include "rasterizer/vanilla.h"

int main(int argc, const char **argv) {
    Vanilla rasterizer(512, 512);

    Eigen::Vector3f eye_pos = {0, 0, 4};
    Camera camera;

    // spot
    // Mesh mesh = obj_loader::LoadObj("model/spot/spot_triangulated_good.obj");
    // mesh.set_model(trans_former::GetTranslation() *
    //                trans_former::GetRotation(140.f) *
    //                trans_former::GetScale(1.5, 1.5, 1.5));

    // bunny
    Mesh mesh = obj_loader::LoadObj("model/bunny/bunny.obj");
    mesh.set_model(trans_former::GetTranslation(0, -1, 0) *
                   trans_former::GetRotation(15) *
                   trans_former::GetScale(10, 10, 10));

    // two triangles
    // Mesh mesh = Mesh({{2,   0,   -2},
    //                   {0,   2,   -2},
    //                   {-2,  0,   -2},
    //                   {3.5, -1,  -5},
    //                   {2.5, 1.5, -5},
    //                   {-1,  0.5, -5}},
    //                  {{0, 1, 2},
    //                   {3, 4, 5}});
    // mesh.set_model(trans_former::GetScale(0.5, 0.5, 0.2));

    // std::cout << mesh;

    int key = 0;
    int frame_count = 0;
    while (key != 27) {
        rasterizer.Clear();

        camera.set_view(eye_pos);
        camera.set_projection(45, 1, 0.1, 6);

        rasterizer.Draw(camera, mesh);
        rasterizer.Show();
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}
