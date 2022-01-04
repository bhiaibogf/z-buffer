//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_OBJ_LOADER_H
#define Z_BUFFER_OBJ_LOADER_H

#include <fstream>

#include "model/mesh.h"

namespace obj_loader {
    Mesh LoadObj(const std::string &path);
}

#endif //Z_BUFFER_OBJ_LOADER_H
