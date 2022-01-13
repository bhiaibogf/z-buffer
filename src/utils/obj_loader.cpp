//
// Created by bhiaibogf on 2021/12/31.
//

#include "obj_loader.h"

Mesh obj_loader::LoadObj(const std::string &path) {
    Mesh mesh;
    std::ifstream obj_stream;
    obj_stream.open(path);

    std::string line;
    while (getline(obj_stream, line)) {

        std::stringstream ss;
        ss << line;

        std::string type;
        ss >> type;
        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            mesh.AddVertex({x, y, z});
        } else if (type == "f") {
            int x, y, z;
            std::string slash;
            ss >> x >> slash;
            if (slash[0] == '/') {
                ss >> y >> slash;
                ss >> z >> slash;
            } else {
                y = stoi(slash);
                ss >> z;
            }
            mesh.AddFace({x, y, z});
        } else {
            continue;
        }
    }
    mesh.GetColor();
    std::cout << path << " Loaded." << std::endl;
    mesh.Count();
    return mesh;
}
