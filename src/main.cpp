#include <iostream>

#include <eigen3/Eigen/Eigen>

#include "utils/transformer.h"
#include "utils/obj_loader.h"
#include "object/mesh.h"
#include "rasterizer/vanilla.h"
#include "rasterizer/scan_line.h"

int main() {
    Vanilla vanilla(512, 512);
    ScanLine scan_line(512, 512);

    Eigen::Vector4f eye_pos = {0, 0, 4, 1};
    Camera camera;

    // 69k bunny
    Mesh mesh = obj_loader::LoadObj("model/bunny_69k.obj");
    mesh.set_model(trans_former::GetTranslation(0, -0.3, 0) *
                   trans_former::GetRotation(15) *
                   trans_former::GetScale(0.5, 0.5, 0.5));

    // bunny
    // Mesh mesh = obj_loader::LoadObj("model/bunny.obj");
    // mesh.set_model(trans_former::GetTranslation(0, -1, 0.7) *
    //                trans_former::GetRotation(15) *
    //                trans_former::GetScale(10, 10, 10));

    // spot
    // Mesh mesh = obj_loader::LoadObj("model/spot.obj");
    // mesh.set_model(trans_former::GetTranslation() *
    //                trans_former::GetRotation(140.f) *
    //                trans_former::GetScale(1.5, 1.5, 1.5));

    // cube
    // Mesh mesh = obj_loader::LoadObj("model/cube.obj");
    // mesh.set_model(trans_former::GetTranslation(0, 0, 0) *
    //                trans_former::GetRotation(15) *
    //                trans_former::GetScale(0.5, 0.5, 0.5));

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
    float angle_y = 0, z_translation = 0;
    bool show_depth = true, use_scan_line = true, draw_line = false;
    double alpha = 0.05, fps = 0;

    puts("draw using scan line");
    while (key != 27) {
        auto start = std::chrono::system_clock::now();

        vanilla.Clear();
        scan_line.Clear();

        camera.set_view((trans_former::GetRotation(angle_y) *
                         trans_former::GetTranslation(0, 0, z_translation) *
                         eye_pos).head<3>());
        camera.set_projection(45, 1, 0.1, 6);

        if (use_scan_line) {
            scan_line.Draw(camera, mesh, draw_line);
            scan_line.Show(show_depth);
        } else {
            vanilla.Draw(camera, mesh);
            vanilla.Show(show_depth);
        }

        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> runtime{end - start};

        fps = alpha / runtime.count() + (1 - alpha) * fps;
        if (frame_count++ % 20 == 0) {
            std::cout << "\rFPS: " << fps;
            std::cout.flush();
        }

        key = cv::waitKey(1);
        switch (key) {
            case 'w':
                z_translation -= .1;
                break;
            case 's':
                z_translation += .1;
                break;
            case 'a':
                angle_y -= 5;
                break;
            case 'd':
                angle_y += 5;
                break;
            case 'r':
                angle_y = 0;
                z_translation = 0;
                break;
            case 'v':
                if (use_scan_line) {
                    scan_line.Save(show_depth);
                } else {
                    vanilla.Save(show_depth);
                }
                break;
            case 'c':
                show_depth = !show_depth;
                break;
            case 'o':
                draw_line = !draw_line;
                break;
            case 'l':
                use_scan_line = !use_scan_line;
                if (use_scan_line) {
                    puts("\ndraw using scan line");
                } else {
                    puts("\ndraw using normal z-buffer");
                }
                break;
            default:
                break;
        }
    }

    return 0;
}
